using System.Collections.Generic;
using UnityEngine;

namespace ObjectPermanence
{
    /**
     * Detects PermanenceComponents and calls UpdateVisibilityState, passing in if the are
     * visible to the set observer camera or not.
     */
    public class ObserverComponent : MonoBehaviour
    {
        public struct PermanenceObject
        {
            public PermanenceComponent PermanenceComponent;
            public Renderer RendererComponent;
        }

        [SerializeField] private Camera _observerCamera;
        private List<PermanenceObject> _permanenceObjects;

        public ObserverComponent()
        {
            _observerCamera = null;
            _permanenceObjects = new List<PermanenceObject>();
        }

        private void Awake()
        {
            PermanenceComponent.PermanenceObjectCreated += OnPermanenceObjectCreated;
            PermanenceComponent.PermanenceObjectDestroyed += OnPermanenceObjectDestroyed;
        }

        private void OnDestroy()
        {
            PermanenceComponent.PermanenceObjectCreated -= OnPermanenceObjectCreated;
            PermanenceComponent.PermanenceObjectDestroyed -= OnPermanenceObjectDestroyed;
        }

        private void Update()
        {
            CheckIfPermanenceObjectsAreVisible();
        }

        private void CheckIfPermanenceObjectsAreVisible()
        {
            Plane[] cameraPlanes = GeometryUtility.CalculateFrustumPlanes(_observerCamera);
            foreach (PermanenceObject permanenceObject in _permanenceObjects)
            {
                bool visible = GeometryUtility.TestPlanesAABB(cameraPlanes, permanenceObject.RendererComponent.bounds);
                var visiblilityState = visible ? PermanenceComponent.VisibilityState.Visible : PermanenceComponent.VisibilityState.NotVisible;
                permanenceObject.PermanenceComponent.UpdateVisibilityState(visiblilityState);
            }
        }

        private void OnPermanenceObjectCreated(PermanenceObject permanenceObject)
        {
            _permanenceObjects.Add(permanenceObject);
        }

        private void OnPermanenceObjectDestroyed(PermanenceObject permanenceObject)
        {
            _permanenceObjects.Remove(permanenceObject);
        }
    }
}
