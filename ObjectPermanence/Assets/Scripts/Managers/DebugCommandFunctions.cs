using UnityEngine;
using UnityEngine.SceneManagement;

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

        public static void ResetScene(MonoBehaviour monoBehaviour)
        {
            SceneManager.LoadScene(SceneManager.GetActiveScene().name);
        }
#pragma warning restore IDE0060 // Remove unused parameter
    }
}
