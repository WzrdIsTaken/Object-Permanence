using UnityEngine;
using UnityEngine.SceneManagement;
using UnityEngine.XR.Interaction.Toolkit;

namespace ObjectPermanence
{
    /**
     * The debug commands. Called from DebugConsole.
     * All these commands should take in a MonoBehaviour first. This is for:
     *  - 1) Code style.
     *  - 2) Some commands will need to access the game world, and so can do this through the passed in MonoBehaviour.
     */
    public static class DebugCommandFunctions
    {
        // Note - keep these alphabetical order please :)

#pragma warning disable IDE0060 // Remove unused parameter (some functions don't use "monoBehaviour")
        public static void ChangeScene(MonoBehaviour monoBehaviour, string newSceneName)
        {
            SceneManager.LoadScene(newSceneName);
        }

        public static void KillPlayer(MonoBehaviour monoBehaviour)
        {
            GameObject.FindGameObjectWithTag(Tags.PlayerTag).GetComponent<PlayerHealthComponent>()?.TakeDamage(int.MaxValue);
        }

        public static void ResetScene(MonoBehaviour monoBehaviour)
        {
            SceneManager.LoadScene(SceneManager.GetActiveScene().name);
        }

        public static void ToggleAllEnemies(MonoBehaviour monoBehaviour, bool toggleState)
        {
            DebugManager.Instance.ConditionalLog(toggleState, LogLevel.Info, DebugCategory.Level,
                "Unfortunately you cant find disabled objects in any nice way. So I'm afraid you'll have to manually enable the enemies ):");

            GameObject[] enemies = GameObject.FindGameObjectsWithTag(Tags.EnemyTag);
            foreach (GameObject enemy in enemies)
            {
                enemy.SetActive(toggleState);
            }
        }

        public static void TriggerHaptic(MonoBehaviour monoBehaviour)
        {
            VRUtils.ForeachController(
                (XRBaseController controller) =>
                {
                    const float strength = 1.0f;
                    const float duration = 1.0f;
                    VRUtils.TriggerHaptic(controller, strength, duration);
                });
        }
#pragma warning restore IDE0060 // Remove unused parameter
    }
}
