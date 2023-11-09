using System.Collections.Generic;
using UnityEngine;

namespace ObjectPermanence
{
    /**
     * Now we can have more than one observer component in a scene... gotta do this
     */
    public sealed class ObserverManager : SingletonInstance<ObserverManager>
    {
        public struct PermanenceObject
        {
            public PermanenceComponent PermanenceComponent;
            public Renderer RendererComponent;
        }

        private List<ObserverComponent> _observers;
        private List<PermanenceObject> _permanenceObjects;

        private ObserverManager()
        {
            _observers = new List<ObserverComponent>();
            _permanenceObjects = new List<PermanenceObject>();
        }

        protected override void Awake()
        {
            base.Awake();

            ObserverComponent.ObserverCreated += OnObserverCreated;
            ObserverComponent.ObserverDestroyed += OnObserverDestroyed;
            PermanenceComponent.PermanenceObjectCreated += OnPermanenceObjectCreated;
            PermanenceComponent.PermanenceObjectDestroyed += OnPermanenceObjectDestroyed;
        }

        private void OnDestroy()
        {
            ObserverComponent.ObserverCreated -= OnObserverCreated;
            ObserverComponent.ObserverDestroyed -= OnObserverDestroyed;
            PermanenceComponent.PermanenceObjectCreated -= OnPermanenceObjectCreated;
            PermanenceComponent.PermanenceObjectDestroyed -= OnPermanenceObjectDestroyed;
        }

        private void Update()
        {
            foreach (PermanenceObject permanenceObject in _permanenceObjects)
            {
                bool objectIsVisible = false;
                foreach (ObserverComponent observer in _observers)
                {
                    objectIsVisible = CheckIfPermanenceObjectIsVisibleToObserver(observer, permanenceObject);

                    if (objectIsVisible)
                    {
                        break;
                    }
                }

                var visiblilityState = objectIsVisible ? PermanenceComponent.VisibilityState.Visible : PermanenceComponent.VisibilityState.NotVisible;
                permanenceObject.PermanenceComponent.UpdateVisibilityState(visiblilityState);
            }
        }

        private bool CheckIfPermanenceObjectIsVisibleToObserver(ObserverComponent observer, PermanenceObject permanenceObject)
        {
            bool visible = false;
            bool inside = false;

            {
                Camera observerCamera = observer.GetCamera();
                float observerBoundsExpansion = observer.GetBoundsExpansion();

                Plane[] cameraPlanes = GeometryUtility.CalculateFrustumPlanes(observerCamera);
                Vector3 observerCameraPosition = observerCamera.transform.position;

                Bounds permanenceObjectBounds = permanenceObject.RendererComponent.bounds;
                visible = GeometryUtility.TestPlanesAABB(cameraPlanes, permanenceObjectBounds);

                if (visible)
                {
                    visible &= RaycastToObjectBoundingBoxExtents(observerCameraPosition,
                        permanenceObjectBounds, permanenceObject.PermanenceComponent.gameObject);
                }
                if (visible)
                {
                    permanenceObjectBounds.Expand(observerBoundsExpansion);
                    inside = permanenceObjectBounds.Contains(observerCameraPosition);
                }
            }

            return (visible && !inside);
        }

        private bool RaycastToObjectBoundingBoxExtents(Vector3 rayStart, Bounds objectBounds, GameObject objectSelf)
        {
            // We could iterate through all the bounds.. but this produces the desired behaviour less expensively

            int exclusionLayer = ~(1 << gameObject.layer);
            Physics.Linecast(rayStart, objectBounds.min, out RaycastHit minHit, exclusionLayer);
            Physics.Linecast(rayStart, objectBounds.max, out RaycastHit maxHit, exclusionLayer);

            if (minHit.collider && maxHit.collider)
            {
                if (minHit.collider.gameObject != objectSelf &&
                    maxHit.collider.gameObject != objectSelf)
                {
                    return false;
                }
            }

            return true;
        }

        private void OnObserverCreated(ObserverComponent observer)
        {
            if (!_observers.Contains(observer))
            {
                _observers.Add(observer);
            }
        }

        private void OnObserverDestroyed(ObserverComponent observer)
        {
            if (_observers.Contains(observer))
            {
                _observers.Remove(observer);
            }
        }

        private void OnPermanenceObjectCreated(PermanenceObject permanenceObject)
        {
            if (!_permanenceObjects.Contains(permanenceObject))
            {
                _permanenceObjects.Add(permanenceObject);
            }
        }

        private void OnPermanenceObjectDestroyed(PermanenceObject permanenceObject)
        {
            if (_permanenceObjects.Contains(permanenceObject))
            {
                _permanenceObjects.Remove(permanenceObject);
            }
        }
    }
}
