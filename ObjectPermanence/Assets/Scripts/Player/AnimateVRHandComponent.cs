using UnityEngine;
using UnityEngine.InputSystem;

namespace ObjectPermanence
{
    /**
     * Animates the VR hands.
     * Credit: Valem Tutorials https://youtu.be/8PCNNro7Rt0?feature=shared
     */
    public class AnimateVRHandComponent : MonoBehaviour
    {
        [SerializeField] private InputActionProperty _pinchInput;
        [SerializeField] private InputActionProperty _gripInput;
        [SerializeField] private Animator _handAnimator;

        private void Update()
        {
            float triggerValue = _pinchInput.action.ReadValue<float>();
            _handAnimator.SetFloat("Trigger", triggerValue);

            float gripvalue = _gripInput.action.ReadValue<float>();
            _handAnimator.SetFloat("Grip", gripvalue);
        }
    }
}
