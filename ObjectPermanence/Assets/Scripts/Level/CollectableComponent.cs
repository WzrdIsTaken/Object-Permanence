using System.Collections;
using UnityEngine;

namespace ObjectPermanence
{
    /**
     * The objective in the little demo minigame.
     */
    public class CollectableComponent : MonoBehaviour
    {
        [SerializeField] private bool _collisionCollectOverride;
        [SerializeField] private bool _fadeOutAfterCollect;

        public CollectableComponent()
        {
            _collisionCollectOverride = false;
            _fadeOutAfterCollect = true;
        }

        private void Start()
        {
            if (_collisionCollectOverride)
            {
                DebugManager.Instance.Log(LogLevel.Warning, DebugCategory.Level, // could use IFDEF unity editor?
                    $"The collectable object {gameObject.name} has {nameof(_collisionCollectOverride)} set to true! Remember to set this to false for the final build");
                GetComponent<BoxCollider>().isTrigger = true;
            }
        }

        void OnTriggerEnter(Collider other)
        {
            if (other.CompareTag(Tags.PlayerTag) && _collisionCollectOverride)
            {
                if (!_fadeOutAfterCollect)
                {
                    transform.parent = other.transform;
                }

                Collect();
            }
        }

        public void Collect()
        {
            DebugManager.Instance.Log(LogLevel.Info, DebugCategory.Level, "Player collected a collectable");

            var collectableTrackerComponent = FindFirstObjectByType<CollectableTrackerComponent>();
            if (collectableTrackerComponent)
            {
                collectableTrackerComponent.Collect();
            }

            GetComponent<BobComponent>().enabled = false;
            GetComponent<Rigidbody>().useGravity = true;
            transform.parent = null;

            if (_fadeOutAfterCollect)
            {
                StartCoroutine(FadeOut());
            }
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
