using System;
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
        public static event Action<ObserverComponent> ObserverCreated;
        public static event Action<ObserverComponent> ObserverDestroyed;

        [SerializeField] private Camera _observerCamera;
        [SerializeField] private float _boundsInsideCheckExpansionAmount;

        public ObserverComponent()
        {
            _observerCamera = null;
            _boundsInsideCheckExpansionAmount = 0.75f;       
        }

        private void OnEnable()
        {
            ObserverCreated?.Invoke(this);
        }

        private void OnDisable()
        {
            ObserverDestroyed?.Invoke(this);
        }

        public Camera GetCamera()
        {
            return _observerCamera;
        }

        public float GetBoundsExpansion()
        {
            return _boundsInsideCheckExpansionAmount;
        }
    }
}
