using System.Collections;
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
            // TODO comment this out when we are just using VR

            if (other.CompareTag(Tags.PlayerTag))
            {
                Collect();
            }
        }

        public void Collect()
        {
            DebugManager.Instance.Log(LogLevel.Info, DebugCategory.Level, "Player collected a collectable");
            FindFirstObjectByType<CollectableTrackerComponent>()?.Collect();

            StartCoroutine(FadeOut());
        }

        private IEnumerator FadeOut()
        {
            Renderer renderer = GetComponent<Renderer>();
            Color initialColor = renderer.material.color;
            Color targetColor = new Color(initialColor.r, initialColor.g, initialColor.b, 0.0f);

            const float fadeDuration = 1.0f;
            float elapsedTime = 0f;

            while (elapsedTime < fadeDuration)
            {
                elapsedTime += Time.deltaTime;
                renderer.material.color = Color.Lerp(initialColor, targetColor, elapsedTime / fadeDuration);
                yield return null;
            }

            Destroy(gameObject);
        }
    }
}
