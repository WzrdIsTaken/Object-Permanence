using UnityEngine;
using UnityEngine.InputSystem;
using UnityEngine.XR.Interaction.Toolkit;

namespace ObjectPermanence
{
    /**
     * Means the ray is not active all of the time.
     */
    public class ActivateTeleportationRayComponent : MonoBehaviour
    {
        [System.Serializable]
        private struct RayInfo
        {
            public GameObject TeleportationRay;
            public InputActionProperty ActivateRayKey;
            public XRRayInteractor RayInteractor;
        }

        [SerializeField] RayInfo _rightRay;
        [SerializeField] RayInfo _leftRay;

        public ActivateTeleportationRayComponent()
        {
            _rightRay = default;
            _leftRay = default;
        }

        private void Update()
        {
            CheckAndToggleRay(_rightRay);
            CheckAndToggleRay(_leftRay);
        }

        private void CheckAndToggleRay(RayInfo ray)
        {
            // If problem see https://youtu.be/0xt6dACM_1I?list=PLpEoiloH-4eP-OKItF8XNJ8y8e1asOJud&t=763. Might need a cancel action.

            bool setRayActive = true;
            setRayActive &= ray.ActivateRayKey.action.ReadValue<float>() > 0.1f;
            setRayActive &= !ray.RayInteractor.TryGetHitInfo(out Vector3 _, out Vector3 _, out int _, out bool _);
            
            ray.TeleportationRay.SetActive(setRayActive);
        }
    }
}
