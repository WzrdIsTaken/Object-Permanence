using UnityEngine;

namespace ObjectPermanence
{
    /**
     * Adds a little bob up and down to make an item look more appealing. 
     */
    public class BobComponent : MonoBehaviour
    {
        [SerializeField] private Transform _transform;
        [SerializeField] float _posSpeed, _posAmount, _rotSpeed, _rotAmount;

        public BobComponent()
        {
            _transform = null;
            // they all equal default right c# magic
        }

        private void Update()
        {
            TransformUtils.SineWaveBob(_transform.gameObject, _transform, _posSpeed, _posAmount, _rotSpeed, _rotAmount);
        }
    }
}
