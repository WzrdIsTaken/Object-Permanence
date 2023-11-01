using UnityEngine;

namespace ObjectPermanence
{
    /**
     * The objective in the little demo minigame.
     */
    public class CollectableComponent : MonoBehaviour
    {
        public CollectableComponent()
        {
        }

        void OnTriggerEnter(Collider other)
        {
            if (other.CompareTag(Tags.PlayerTag))
            {
                Collect();
            }
        }

        private void Collect()
        {
            DebugManager.Instance.Log(LogLevel.Info, DebugCategory.Level, "Player collected a collectable");
            Destroy(gameObject);
        }
    }
}
