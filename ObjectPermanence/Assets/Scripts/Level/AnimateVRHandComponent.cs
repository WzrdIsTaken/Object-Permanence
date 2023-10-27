using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.InputSystem;

namespace ObjectPermanence
{
    /**
     * TODO desc and link tut
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
