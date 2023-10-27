using UnityEngine;

namespace ObjectPermanence
{
    /**
     * The motivation for this script was needing a seperate camera for the observer, but it still 
     * needs to rotate with the vr camera.
     */
    public class MirrorTransformComponent : MonoBehaviour
    {
        [SerializeField] private Transform _targetTransform;
        [SerializeField] private Transform _thisTransform;

        [SerializeField] private bool _mirrorPosition;
        [SerializeField] private bool _mirrorRotation;

        public MirrorTransformComponent()
        {
            _targetTransform = null;
            _thisTransform = null;
            _mirrorPosition = false;
            _mirrorRotation = false;
        }

        private void Update()
        {
            if (_mirrorPosition)
            {
                _thisTransform.position = _targetTransform.position;
            }
            if (_mirrorRotation)
            {
                _thisTransform.rotation = _targetTransform.rotation;
            }
        }
    }
}
