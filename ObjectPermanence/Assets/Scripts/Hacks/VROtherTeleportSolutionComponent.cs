using UnityEngine;
using UnityEngine.InputSystem;

namespace ObjectPermanence
{
    /**
     * ???
     */
    public class VROtherTeleportSolutionComponent : MonoBehaviour
    {
        [System.Serializable]
        private class TeleportInfo
        {
            public InputActionProperty TeleportKey;
            public GameObject Reticle;
            [HideInInspector] public bool Teleported = false;
        }

        [SerializeField] private TeleportInfo _right;
        [SerializeField] private TeleportInfo _left;

        public VROtherTeleportSolutionComponent()
        {
            _right = null;
            _left = null;
        }

        private void Update()
        {
            CheckAndTeleport(_right);
            CheckAndTeleport(_left);
        }

        private void CheckAndTeleport(TeleportInfo info)
        {
            if (info.TeleportKey.action.ReadValue<float>() > 0.99f && !info.Teleported)
            {
                transform.position = info.Reticle.transform.position;
                info.Teleported = true;
            }
            else
            {
                info.Teleported = false;
            }
        }
    }
}
