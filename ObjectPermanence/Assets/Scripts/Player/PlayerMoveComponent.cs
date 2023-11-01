using UnityEngine;

namespace ObjectPermanence
{
    /**
     * A super basic component that alls the player to move around.
     * Credit: Brackeys https://youtu.be/_QajrabyTJc?feature=shared
     */
    public class PlayerMoveComponent : MonoBehaviour
    {
        private struct MoveInput
        {
            public float X;
            public float Z;
            public bool Jump;
        }

        [SerializeField] private CharacterController _controller;
        [SerializeField] private Transform _groundCheck;

        [SerializeField] private float _moveSpeed;
        [SerializeField] private float _jumpHeight;
        [SerializeField] private float _gravityMultiplier;
        [SerializeField] private float _groundDistance;
        [SerializeField] private LayerMask _groundMask;

        private Vector3 _velocity;
        private float _gravity; // Note - this (currently..) isn't updated in OnValidate
        private bool _isGrounded;

        public PlayerMoveComponent()
        {
            _controller = null;
            _groundCheck = null;

            _moveSpeed = 12.0f;
            _jumpHeight = 3.0f;
            _gravityMultiplier = 2.0f;
            _groundDistance = 0.25f;
            _groundMask = default;

            _velocity = default;
            _gravity = WorldConstants.Gravity * _gravityMultiplier;
            _isGrounded = false;
        }

        private void Update()
        {
            MoveInput input = GetInput();

            GroundedCheck();
            MoveHorizontal(input);
            MoveVertical(input);   
        }

        private MoveInput GetInput()
        {
            MoveInput moveInput = default;
            moveInput.X = Input.GetAxis(Inputs.MoveX);
            moveInput.Z = Input.GetAxis(Inputs.MoveZ);
            moveInput.Jump = Input.GetButtonDown(Inputs.Jump);

            return moveInput;
        }

        private void GroundedCheck()
        {
            _isGrounded = Physics.CheckSphere(_groundCheck.position, _groundDistance, _groundMask);
            if (_isGrounded && _velocity.y < 0.0f)
            {
                _velocity.y = -2.0f;
            }
        }

        private void MoveHorizontal(MoveInput input)
        {
            Vector3 move = (transform.right * input.X) + (transform.forward * input.Z);
            move *= _moveSpeed * Time.deltaTime;
            _controller.Move(move);
        }

        private void MoveVertical(MoveInput input)
        {
            if (input.Jump && _isGrounded)
            {
                _velocity.y = Mathf.Sqrt(_jumpHeight * -2.0f * _gravity);
            }

            _velocity.y += _gravity * Time.deltaTime;
            _controller.Move(_velocity * Time.deltaTime);
        }
    }
}
