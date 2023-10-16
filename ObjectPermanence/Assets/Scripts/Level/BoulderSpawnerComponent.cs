using System.Collections;
using UnityEngine;

namespace ObjectPermanence
{
    /**
     * Creates a new boulder each time one surpasses the _boulderThreshold.
     */
    public class BoulderSpawnerComponent : MonoBehaviour
    {
        [SerializeField] private GameObject _boulderPrefab;
        [SerializeField] private Transform _boulderThreshold;
        [SerializeField] private float _boulderForce;

        private GameObject _lastBoulder;

        public BoulderSpawnerComponent()
        {
            _boulderPrefab = null;
            _boulderThreshold = null;
            _boulderForce = 10.0f;
            _lastBoulder = null;
        }

        private void Update()
        {
            if (!_lastBoulder)
            {
                _lastBoulder = SpawnBoulder();
            }
            else
            {
                float thresholdDistance = Vector3.Distance(transform.position, _boulderThreshold.position);
                float lastBoulderDistance = Vector3.Distance(transform.position, _lastBoulder.transform.position); // Perhaps calc this in start/onvalidate?

                if (thresholdDistance < lastBoulderDistance)
                {
                    _lastBoulder = null;
                }
            }
        }

        private GameObject SpawnBoulder()
        {
            GameObject boulder = Instantiate(_boulderPrefab, transform.position, transform.rotation, transform);
            boulder.GetComponent<Rigidbody>().AddForce((transform.forward * _boulderForce), ForceMode.Impulse);

            return boulder;
        }
    }
}
