using UnityEngine;

namespace ObjectPermanence
{
    /**
     * Destroys a game object when the assigned condition is met.
     */
    public class DestroyOnCondition : MonoBehaviour
    {
        [System.Flags]
        private enum DestroyCondition
        {
            YAxisLimit
            // Any other conditions here!
        }
        [SerializeField] private DestroyCondition _destroyCondition;

        private void Update()
        {
            switch (_destroyCondition)
            {
                case DestroyCondition.YAxisLimit:
                    CheckYAxisLimitCondition();
                    break;
                default:
                    DebugManager.Instance.AssertFail(AssertLevel.Assert, DebugCategory.Level,
                            $"Using an unexpected {nameof(DestroyCondition)}");
                    break;
            }
        }

        private void CheckYAxisLimitCondition()
        {
            if (transform.position.y < WorldConstants.DestroyY)
            {
                DestroyObject();
            }
        }

        private void DestroyObject()
        {
            if (gameObject.CompareTag(Tags.PlayerTag))
            {
                GetComponent<PlayerHealthComponent>().TakeDamage(int.MaxValue);
            }
            else
            {
                Destroy(gameObject);
            }
        }
    }
}
