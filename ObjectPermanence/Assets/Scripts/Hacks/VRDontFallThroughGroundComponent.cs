using UnityEngine;
using UnityEngine.XR.Interaction.Toolkit;

namespace ObjectPermanence
{
    /**
     * im just there is just one checkbox im missing or something
     * edit - found it....
     */
    public class VRDontFallThroughGroundComponent : MonoBehaviour
    {
        [SerializeField] private ActionBasedContinuousMoveProvider _moveProvider;
        [SerializeField] private Transform _groundCheck;
        [SerializeField] private float _groundDistance;
        [SerializeField] private LayerMask _groundMask;

        public VRDontFallThroughGroundComponent()
        {
            _moveProvider = null;
            _groundCheck = null;
            _groundDistance = 1.0f;
            _groundMask = default;
        }

        private void Start()
        {
            _moveProvider.useGravity = false;
        }

        private void Update()
        {
            bool isGrounded = Physics.CheckSphere(_groundCheck.position, _groundDistance, _groundMask);
            if (!isGrounded && !_moveProvider.useGravity)
            {
                _moveProvider.useGravity = true;
            }
            else if (isGrounded && _moveProvider.useGravity)
            {
                _moveProvider.useGravity = false;
            }
        }
    }
}
