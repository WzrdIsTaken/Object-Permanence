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
        //[SerializeField] InputActionProperty _cancel;

        void Update()
        {
            // If problem see https://youtu.be/0xt6dACM_1I?list=PLpEoiloH-4eP-OKItF8XNJ8y8e1asOJud&t=763
            _teleportationRay.SetActive(/*_cancel.action.ReadValue<float>() == 0.0f &&*/_activateRay.action.ReadValue<float>() > 0.1f);
        }
    }
}
