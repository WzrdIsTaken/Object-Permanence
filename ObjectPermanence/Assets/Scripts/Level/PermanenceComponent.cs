using System;
using System.Collections.Generic;
using System.Linq;
using System.Reflection;
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
     *  - Not all components having a enabled property, and it doesn't seem custom classes can be casted to DynamicObject
     *    (I think...), which is why I have to do this reflection stuff. I think a better solution would be to write an 
     *    extension property for the components which don't have "enabled", but it doesn't look like thats a feature.
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
        private Rigidbody _rigidbody;
        private List<dynamic> _components;

        public PermanenceComponent()
        {
            _visibilityState = VisibilityState.NotSet;
            _onBecomeVisible = OnBecomeVisible.Activate;
            _overrideComponents = new List<Component>();

            _permanenceObject = default;
            _rigidbody = null;
            _components = new List<dynamic>();
        }

        private void Start()
        {
            InitValidComponentsOnGameObject();
            InitRigidbody();
            InitPermanenceObject();
            UpdateVisibilityState(_visibilityState);

            PermanenceObjectCreated?.Invoke(_permanenceObject);
        }

        private void OnDestroy()
        {
            PermanenceObjectDestroyed?.Invoke(_permanenceObject);
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

        private void InitValidComponentsOnGameObject()
        {
            _components = GetComponents<Component>()
                .Select(c => (dynamic)c)
                .Where(c => c != this && !_overrideComponents.Contains(c))
                .ToList();

            _components
                .RemoveAll(c =>
                {
                    bool validComponent = false;

                    Type type = c.GetType();
                    PropertyInfo[] properties = type.GetProperties();
                    validComponent = properties.Any(p => p.Name == "enabled");

                    return !validComponent; // RemoveAll removes entries which match the predicate
                });
        }

        private void InitPermanenceObject()
        {
            _permanenceObject.PermanenceComponent = this;
            _permanenceObject.RendererComponent = GetComponent<Renderer>();
        }

        private void InitRigidbody()
        {
            TryGetComponent<Rigidbody>(out _rigidbody);
            if (_rigidbody && _overrideComponents.Contains(_rigidbody))
            {
                _rigidbody = null;
            }
        }

        private void ToggleComponents(bool toggleState)
        {
            if (_rigidbody) // Rigidbody does not have an enabled property but we need to handle it
            {
                _rigidbody.isKinematic = !toggleState;
                _rigidbody.detectCollisions = toggleState;
            }

            foreach (dynamic component in _components)
            {
                component.enabled = toggleState;
            }
        }
    }
}
