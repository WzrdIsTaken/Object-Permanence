using System;
using System.Collections.Generic;
using System.Linq;
using UnityEngine;

namespace ObjectPermanence
{
    /**
     * Gameobjects with this component are detected by the ObserverComponent. If they are not visible, 
     * this component will disable all components on the object apart from those listed in _overrideComponents.
     * 
     * Notes:
     *  - This solution doesn't currently handle if a component is added to the gameobject at runtime. There doesn't
     *    seem to be an OnComponentAdded/Removed event I can hook into (which is how I would handle this), so I will
     *    need to write my own extension method and override AddComponent/RemoveComponent. However, right now I don't
     *    think this issue will be a problem in this project.
     *      - But, if we need to fix it a hacky way is just to call InitValidComponentsOnGameObject in UpdateVisibilityState.
     */
    public class PermanenceComponent : MonoBehaviour
    {
        public static event Action<ObserverComponent.PermanenceObject> PermanenceObjectCreated;
        public static event Action<ObserverComponent.PermanenceObject> PermanenceObjectDestroyed;

        public enum VisibilityState
        {
            Visible,
            NotVisible,
            NotSet
        }

        private enum OnBecomeVisible
        {
            Activate,
            Deactivate
        }

        private VisibilityState _visibilityState;
        [SerializeField] private OnBecomeVisible _onBecomeVisible;
        [SerializeField] private List<Component> _overrideComponents;
       
        private ObserverComponent.PermanenceObject _permanenceObject;
        private List<dynamic> _components; // Component types must be preserved so the correction version of ComponentUtils.ToggleComponent is called

        public PermanenceComponent()
        {
            _visibilityState = VisibilityState.NotSet;
            _onBecomeVisible = OnBecomeVisible.Activate;
            _overrideComponents = new List<Component>();

            _permanenceObject = default;
            _components = new List<dynamic>();
        }

        private void Start()
        {
            RefreshState();
            PermanenceObjectCreated?.Invoke(_permanenceObject);
        }

        private void OnDestroy()
        {
            PermanenceObjectDestroyed?.Invoke(_permanenceObject);
        }

        private void OnValidate()
        {
            RefreshState();
        }

        public void UpdateVisibilityState(VisibilityState visibilityState)
        {
            if (visibilityState != _visibilityState)
            {
                _visibilityState = visibilityState;
                bool visible = (visibilityState == VisibilityState.Visible);

                switch (_onBecomeVisible)
                {
                    case OnBecomeVisible.Activate:
                        ToggleComponents(visible);
                        break;
                    case OnBecomeVisible.Deactivate:
                        ToggleComponents(!visible);
                        break;
                    default:
                        DebugManager.Instance.AssertFail(AssertLevel.Assert, DebugCategory.Level,
                            $"Using an unexpected {nameof(OnBecomeVisible)}");
                        break;
                }

                DebugManager.Instance.Log(LogLevel.Info, DebugCategory.Level,
                    $"The gameobject \"{gameObject.name}\" is now {(visible ? "visible" : "not visible")} to the observer camera");
            }
        }

        private void RefreshState()
        {
            _visibilityState = VisibilityState.NotSet;
            _components.Clear();

            InitValidComponentsOnGameObject();
            InitPermanenceObject();
            UpdateVisibilityState(_visibilityState);
        }

        private void InitValidComponentsOnGameObject()
        {
            _components = GetComponents<Component>()
                .Select(c => (dynamic)c)
                .Where(c => c != this && !_overrideComponents.Contains(c))
                .ToList();

            /*
             * Previously I did this reflection to find if a component can be enabled/disabled.
             * It has its downsides so was removed, but I still think it was kinda neat.
             * 
                _components
                    .RemoveAll(c =>
                    {
                        bool validComponent = false;

                        Type type = c.GetType();
                        PropertyInfo[] properties = type.GetProperties();
                        validComponent = properties.Any(p => p.Name == "enabled");

                        return !validComponent; // RemoveAll removes entries which match the predicate
                    });
            */
        }

        private void InitPermanenceObject()
        {
            _permanenceObject.PermanenceComponent = this;
            _permanenceObject.RendererComponent = GetComponent<Renderer>();
        }

        private void ToggleComponents(bool toggleState)
        {
            foreach (dynamic component in _components)
            {
                ComponentUtils.ToggleComponent(component, toggleState);
            }
        }
    }
}
