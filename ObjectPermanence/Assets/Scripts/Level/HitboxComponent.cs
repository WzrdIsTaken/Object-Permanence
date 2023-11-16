using UnityEngine;

namespace ObjectPermanence
{
    /**
     * Detects collision with the player and deals damage.
     */
    public class HitboxComponent : MonoBehaviour
    {
        [SerializeField] int _damage;

        public HitboxComponent()
        {
            _damage = 1;
        }

        private void OnTriggerEnter(Collider other)
        {
            if (other.CompareTag(Tags.PlayerTag))
            {
                PlayerHealthComponent playerHealthComponent = other.GetComponent<PlayerHealthComponent>() ?? other.GetComponentInParent<PlayerHealthComponent>();
                playerHealthComponent.TakeDamage(_damage);
            }
        }
    }
}
