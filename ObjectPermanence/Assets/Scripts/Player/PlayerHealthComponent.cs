using UnityEngine;
using UnityEngine.SceneManagement;

namespace ObjectPermanence
{
    /**
     * Manages the players 'hp' and getting hit.
     */
    public class PlayerHealthComponent : MonoBehaviour
    {
        [SerializeField] int _maxHealth;
        private int _health;
        private bool _dead;

        public PlayerHealthComponent()
        {
            _maxHealth = 1;
            _health = _maxHealth;
            _dead = false;
        }

        private void Start()
        {
            _health = _maxHealth;
        }

        public void TakeDamage(int damage)
        {
            _health -= damage;
            if (_health <= 0)
            {
                Die();
            }
        }

        private void Die()
        {
            if (!_dead)
            {
                DebugManager.Instance.Log(LogLevel.Info, DebugCategory.Level, "Player has died");
                AudioManager.Instance.PlayEffect(AudioID.Reset, AudioMixerID.SFX, AudioPlaySettings.Default);
                SceneManager.LoadScene(SceneManager.GetActiveScene().name);
                _dead = true;
            }
        }
    }
}
