using UnityEngine;

namespace ObjectPermanence
{
    public class ApplyForceOnSpawnComponent : MonoBehaviour
    {
        [SerializeField] private float _force;
        [SerializeField] private float _delay;

        private void Start()
        {
            Invoke(nameof(ApplyForce), _delay);
        }

        private void ApplyForce()
        {
            GetComponent<Rigidbody>().AddForce((transform.forward * _force), ForceMode.Impulse);
        }
    }
}
