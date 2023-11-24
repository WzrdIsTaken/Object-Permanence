using UnityEngine;
using UnityEngine.SceneManagement;

namespace ObjectPermanence
{
    /**
     * ki bun e an jo wa ):
     */
    public class WonGameDetectionComponent : MonoBehaviour
    {
        [SerializeField] private GameObject _winText;
        private bool _won = false;

        public void Update()
        {
            if (_won)
            {
                bool debugKeyboardInput = Input.GetKeyDown(KeyCode.R);
                bool vrInput = false; // TODO impl - this has gotta be event based right?

                if (debugKeyboardInput || vrInput)
                {
                    SceneManager.LoadScene(SceneManager.GetActiveScene().name);
                }
            }
        }

        private void OnTriggerEnter(Collider other)
        {
            if (other.CompareTag(Tags.CollectableTag))
            {
                if (!_won)
                {
                    _winText.SetActive(true);
                    _won = true;

                    DebugManager.Instance.Log(LogLevel.Info, DebugCategory.Level, "The player has won!");
                }
            }
        }
    }
}
