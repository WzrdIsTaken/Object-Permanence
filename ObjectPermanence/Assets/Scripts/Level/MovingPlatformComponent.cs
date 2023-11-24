using UnityEngine;

namespace ObjectPermanence
{
    /**
     * Moves up and down
     */
    public class MovingPlatformComponent : MonoBehaviour
    {
        [SerializeField] private float _maxHeight;
        [SerializeField] private float _moveSpeed;

        private float _maxY;
        private float _minY;
        private bool _moveUp = true;
        private bool _moveOverride = false;

        private void Start()
        {
            _maxY = transform.position.y + _maxHeight;
            _minY = transform.position.y;
            _moveUp = true;
        }

        private void Update()
        {
            if (_moveOverride) return;

            float currentY = transform.position.y;

            if (_moveUp)
            {
                if (currentY < _maxY)
                {
                    currentY += _moveSpeed * Time.deltaTime;
                }
            }
            else
            {
                if (currentY > _minY)
                {
                    currentY -= _moveSpeed * Time.deltaTime;
                }
            }

            transform.position = new Vector3(transform.position.x, currentY, transform.position.z);
        }

        public void Activate()
        {
            _moveUp = false;
        }

        public void Deactivate()
        {
            _moveUp = true;
        }

        private void OnTriggerEnter(Collider other)
        {
            if (other.CompareTag(Tags.PlayerTag))
            {
                _moveOverride = true;
            }
        }

        private void OnTriggerExit(Collider other)
        {
            if (other.CompareTag(Tags.PlayerTag))
            {
                _moveOverride = false;
            }
        }
    }
}
