using UnityEngine;
using UnityEngine.SceneManagement;
using UnityEngine.XR.Interaction.Toolkit;

namespace ObjectPermanence
{
    /**
     * Manages the players 'hp' and getting hit.
     */
    public class PlayerHealthComponent : MonoBehaviour
    {
        [SerializeField] int _maxHealth;
        [SerializeField] float _deathHapticStregth;
        [SerializeField] float _deathHapticDuration;

        private int _health;
        private bool _dead;

        public PlayerHealthComponent()
        {
            _maxHealth = 1;
            _deathHapticStregth = 1.0f;
            _deathHapticDuration = 0.5f;

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

                VRUtils.ForeachController(
                    (XRBaseController controller) =>
                    {
                        VRUtils.TriggerHaptic(controller, _deathHapticStregth, _deathHapticDuration);
                    });

                _dead = true;
            }
        }
    }
}
