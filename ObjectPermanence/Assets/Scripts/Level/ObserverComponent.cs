using System.Collections.Generic;
using UnityEngine;

namespace ObjectPermanence
{
    /**
     * Detects PermanenceComponents and calls UpdateVisibilityState, passing in if the are
     * visible to the set observer camera or not.
     * 
     * Notes:
     *  - We need the inside check to reduce the jitteriness when moving backwards through objects. 
     *    Otherwise, when we are inside the object it becomes visible to the camera and the collider 
     *    gets enabled. I found that increasing the bounds just a little when doing this check 
     *    helped make it smoother.
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
        private readonly float _boundsInsideCheckExpansionAmount;

        public ObserverComponent()
        {
            _observerCamera = null;
            _permanenceObjects = new List<PermanenceObject>();
            _boundsInsideCheckExpansionAmount = 0.75f; 
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
                Bounds permanenceObjectBounds = permanenceObject.RendererComponent.bounds;
                bool visible = GeometryUtility.TestPlanesAABB(cameraPlanes, permanenceObjectBounds);

                permanenceObjectBounds.Expand(_boundsInsideCheckExpansionAmount);
                bool inside = permanenceObjectBounds.Contains(_observerCamera.transform.position);

                var visiblilityState = (visible && !inside) ? PermanenceComponent.VisibilityState.Visible : PermanenceComponent.VisibilityState.NotVisible;
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
