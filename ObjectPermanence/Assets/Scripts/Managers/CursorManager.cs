using UnityEngine;

namespace ObjectPermanence
{
    /**
     * Allows the cursor state to be set.
     */
    public sealed class CursorManager : Singleton<CursorManager>
    {
        [SerializeField] private CursorLockMode _cursorLockMode;

        private CursorManager()
        {
            _cursorLockMode = CursorLockMode.None;
        }

        private void Start()
        {
            RefreshState();
        }

        private void OnValidate()
        {
            RefreshState();
        }

        public CursorLockMode GetCursorLockMode()
        {
            return _cursorLockMode;
        }

        public void SetCursorLockMode(CursorLockMode cursorlockMode)
        {
            _cursorLockMode = cursorlockMode;
            RefreshState();
        }

        private void RefreshState()
        {
            Cursor.lockState = _cursorLockMode;
        }
    }
}
