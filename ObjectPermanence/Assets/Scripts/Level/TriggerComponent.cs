using System.Collections.Generic;
using UnityEngine;


namespace ObjectPermanence
{
    public class TriggerComponent : MonoBehaviour
    {
        [SerializeField] List<GameObject> _objectsToTrigger;
        [SerializeField] bool _onlyToggleOnce;
        int _toggleCount;

        public TriggerComponent()
        {
            _objectsToTrigger = new List<GameObject>();
            _onlyToggleOnce = true;
            _toggleCount = 0;
        }

        private void OnTriggerEnter(Collider other)
        {
            if (other.CompareTag(Tags.PlayerTag))
            {
                Trigger();
            }
        }

        private void Trigger()
        {
            if (_onlyToggleOnce && _toggleCount != 0)
            {
                return;
            }

            foreach (GameObject gameObject in _objectsToTrigger)
            {
                gameObject.SetActive(!gameObject.activeSelf);
            }
            _toggleCount++;
        }
    }
}
