using UnityEngine;

namespace ObjectPermanence
{
    /**
     * The singleton instance overrides the current instance instead of destroying any new ones.
     */
    public abstract class SingletonInstance<T> : MonoBehaviour where T : MonoBehaviour
    {
        public static T Instance { get; private set; }

        protected virtual void Awake()
        {
            Instance = this as T;
        }

        protected virtual void OnApplicationQuit()
        {
            Instance = null;
            Destroy(gameObject);
        }
    }

    /**
     * Basic singleton. Use this in most cases as managers should be loaded via the Bootstrapper.
     */
    public abstract class Singleton<T> : SingletonInstance<T> where T : MonoBehaviour
    {
        protected override void Awake()
        {
            if (Instance != null)
            {
                Destroy(gameObject);
            }
            base.Awake();
        }
    }

    /**
     * This version of the singleton will persist through scene loads. Note if is not needed if the singleton is being loaded via the Bootstrapper.
     */
    public abstract class PersistentSingleton<T> : Singleton<T> where T : MonoBehaviour
    {
        protected override void Awake()
        {
            base.Awake();
            DontDestroyOnLoad(gameObject);
        }
    }
}
