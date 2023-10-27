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
        [SerializeField] private InputActionProperty _pinchAnimationAction;
        [SerializeField] private InputActionProperty _gripAnimationAction;
        [SerializeField] private Animator _handAnimator;

        private void Update()
        {
            float triggerValue = _pinchAnimationAction.action.ReadValue<float>();
            _handAnimator.SetFloat("Trigger", triggerValue);

            float gripvalue = _gripAnimationAction.action.ReadValue<float>();
            _handAnimator.SetFloat("Grip", gripvalue);
        }
    }
}
