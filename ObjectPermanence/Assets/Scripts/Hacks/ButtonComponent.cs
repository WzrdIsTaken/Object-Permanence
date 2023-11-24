using UnityEngine;

namespace ObjectPermanence
{
    // experimenting with writing the worst code i can.. 
    public class ButtonComponent : MonoBehaviour
    {
        [SerializeField] bool _otherBehaviour = false;

        static int _enabledCount = 0;

        [SerializeField] GameObject _door;
        [SerializeField] Material _enabledMat;
        [SerializeField] Material _disabledMat;

        void OnEnable()
        {
            GetComponent<MeshRenderer>().material = _enabledMat;

            if (!_otherBehaviour)
            {
                ++_enabledCount;
            }
            else
            {
                GameObject.FindObjectOfType<MovingPlatformComponent>().Activate();
            }
        }

        void OnDisable()
        {
            GetComponent<MeshRenderer>().material = _disabledMat;

            if (!_otherBehaviour)
            {
                --_enabledCount;
            }
            else
            {
                GameObject.FindObjectOfType<MovingPlatformComponent>().Deactivate();
            }
        }

        void Update()
        {
            if (!_otherBehaviour && _door) _door.SetActive(_enabledCount != 2);
        }
    }
}
