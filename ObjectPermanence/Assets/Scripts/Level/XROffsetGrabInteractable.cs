using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.XR.Interaction.Toolkit;

namespace ObjectPermanence
{
    /**
     * vrrrrrr 
     */
    public class XROffsetGrabInteractable : XRGrabInteractable
    {
        private Vector3 _initialLocalPos;
        private Quaternion _initialLocalRot;

        public XROffsetGrabInteractable()
        {
            _initialLocalPos = default;
            _initialLocalRot = default;
        }

        private void Start()
        {
            if (!attachTransform)
            {
                var attachPoint = new GameObject("Offset Grab Point");
                const bool worldPositionStays = false;
                attachPoint.transform.SetParent(transform, worldPositionStays);
                attachTransform = attachPoint.transform;
            }
            else
            {
                _initialLocalPos = attachTransform.localPosition;
                _initialLocalRot = attachTransform.localRotation;
            }
        }

        protected override void OnSelectEntered(SelectEnterEventArgs args)
        {
            // Picking up an object
            if (args.interactableObject is XRDirectInteractor)
            {
                attachTransform.SetPositionAndRotation(args.interactableObject.transform.position, args.interactableObject.transform.rotation);
            }
            // Using the ray
            else
            {
                attachTransform.SetLocalPositionAndRotation(_initialLocalPos, _initialLocalRot);
            }

           

            base.OnSelectEntered(args);
        }
    }
}
