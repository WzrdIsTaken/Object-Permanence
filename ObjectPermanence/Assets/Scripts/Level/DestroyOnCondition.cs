using UnityEngine;

namespace ObjectPermanence
{
    /**
     * Destroys a game object when the assigned condition is met.
     */
    public class DestroyOnCondition : MonoBehaviour
    {
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
            // Pulled this logic out in case we ever want to do anything special...
            Destroy(gameObject);
        }
    }
}
