using UnityEngine;
using UnityEngine.InputSystem;

namespace ObjectPermanence
{
    /**
     * Means the ray is active all of the time.
     */
    public class ActivateTeleportationRayComponent : MonoBehaviour
    {
        [SerializeField] GameObject _teleportationRay;
        [SerializeField] InputActionProperty _activateRay;

        void Update()
        {
            _teleportationRay.SetActive(_activateRay.action.ReadValue<float>() > 0.1f);
        }
    }
}
