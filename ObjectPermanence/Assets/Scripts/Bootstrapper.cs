using UnityEngine;

namespace ObjectPermanence
{
    /**
     * Creates resources and marks them with don't destroy on load.  Prefabs stored in Assets/Resources and listed in _ResourceNames will be 
     * instantiated on startup, in whichever scene you're in. This ensures that manager dependencies are always available in an elegant way. 
     * A detailed explanation can be viewed at https://www.youtube.com/watch?v=zJOxWmVveXU 
     */
    public static class Bootstrapper
    {
        private static readonly string[] _resourceNames =
        {
            "Managers"
        };

        [RuntimeInitializeOnLoadMethod(RuntimeInitializeLoadType.BeforeSceneLoad)]
        public static void Execute()
        {
            foreach (string resourceName in _resourceNames)
            {
                Object.DontDestroyOnLoad(Object.Instantiate(Resources.Load(resourceName)));
            }
        }
    }
}
