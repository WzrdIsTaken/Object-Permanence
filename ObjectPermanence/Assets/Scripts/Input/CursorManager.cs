using UnityEngine;

namespace ObjectPermanence
{
    /**
     * Allows the cursor state to be set.
     */
    public sealed class CursorManager : Singleton<CursorManager>
    {
        [SerializeField] private CursorLockMode _cursorLockMode;
        [SerializeField] private bool _cursorVisibile;

        private CursorManager()
        {
            _cursorLockMode = CursorLockMode.None;
            _cursorVisibile = true;
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

        public bool GetCursorVisibile()
        {
            return _cursorVisibile;
        }

        public void SetCursorVisibile(bool cursorVisibile)
        {
            _cursorVisibile = cursorVisibile;
            RefreshState();
        }

        private void RefreshState()
        {
            Cursor.lockState = _cursorLockMode;
            Cursor.visible = _cursorVisibile;
        }
    }
}
