using UnityEngine;

namespace ObjectPermanence
{
    /**
     * A super basic component that alls the player to look around.
     * Credit: Brackeys https://youtu.be/_QajrabyTJc?feature=shared
     */
    public class PlayerLookComponent : MonoBehaviour
    {
        private struct LookInput
        {
            public float X;
            public float Y;
        }

        [SerializeField] private Transform _playerBody;
        [SerializeField] private float _mouseSensitivity;
        private float _xRotation;

        public PlayerLookComponent()
        {
            _playerBody = null;
            _mouseSensitivity = 100.0f;
            _xRotation = 0.0f;
        }

        private void Update()
        {
            LookInput input = GetInput();

            MoveHorizontal(input);
            MoveVertical(input);
        }

        private LookInput GetInput()
        {
            LookInput lookInput = default;
            lookInput.X = Input.GetAxis(Inputs.MouseX) * _mouseSensitivity * Time.deltaTime;
            lookInput.Y = Input.GetAxis(Inputs.MouseY) * _mouseSensitivity * Time.deltaTime;

            return lookInput;
        }

        private void MoveHorizontal(LookInput input)
        {
            _playerBody.Rotate(Vector3.up * input.X);
        }

        private void MoveVertical(LookInput input)
        {
            _xRotation -= input.Y;
            _xRotation = Mathf.Clamp(_xRotation, -90.0f, 90.0f);
            transform.localRotation = Quaternion.Euler(_xRotation, 0.0f, 0.0f);
        }
    }
}
