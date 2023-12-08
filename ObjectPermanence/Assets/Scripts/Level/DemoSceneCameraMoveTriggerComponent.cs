using UnityEngine;

namespace ObjectPermanence
{
    /**
     * Updates the DemoSceneCameraMoverComponent
     */
    public class DemoSceneCameraMoveTriggerComponent : MonoBehaviour
    {
        [SerializeField] Transform _cameraPosition;
        [SerializeField] Transform _lookAtPosition;
        DemoSceneCameraMoverComponent _demoSceneCameraMoverComponent;

        void Start()
        {
            _demoSceneCameraMoverComponent = FindFirstObjectByType<DemoSceneCameraMoverComponent>();
            DebugManager.Instance.Assert(_demoSceneCameraMoverComponent, AssertLevel.Assert, DebugCategory.Level, $"Unable to find a {nameof(DemoSceneCameraMoverComponent)}");
        }

        void OnTriggerEnter(Collider other)
        {
            if (other.CompareTag(Tags.PlayerTag) && _demoSceneCameraMoverComponent)
            {
                _demoSceneCameraMoverComponent.SetCamera(_cameraPosition.position, _lookAtPosition.position);
            }
        }
    }
}

