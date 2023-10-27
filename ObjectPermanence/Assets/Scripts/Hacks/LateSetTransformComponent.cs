
using UnityEngine;

namespace ObjectPermanence
{
    /**
     * Something is setting the VR camera height... I think I know what it is but I can't edit it. 
     * There probs is a value in the inspector I can change but I can't see it, so I wrote this bot script.
     * Hey, this project is meant to be a quick prototype :p
     */
    public class LateSetTransformComponent : MonoBehaviour
    {
        [SerializeField] private Transform _targetTransform;
        [SerializeField] private Vector3 _position;
        private bool _set;

        public LateSetTransformComponent()
        {
            _targetTransform = null;
            _position = default;
            _set = false;
        }

        private void Update()
        {
            if (!_set)
            {
                _targetTransform.position = _position;
                _set = true;
            }
        }
    }
}
