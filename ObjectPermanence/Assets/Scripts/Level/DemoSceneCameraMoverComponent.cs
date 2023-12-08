using UnityEngine;
using UnityEditor;

namespace ObjectPermanence
{
    /**
     * Moves the camera to set points in the scene view for the demo (its easier to see whats going on w/ 2 cameras / in editor)
     */
    public class DemoSceneCameraMoverComponent : MonoBehaviour
    {
        Vector3 _cameraPosition;
        Vector3 _cameraLookAtPosition;

        void Update()
        {
#if UNITY_EDITOR
            SceneView.lastActiveSceneView.camera.transform.position = _cameraPosition;
            SceneView.lastActiveSceneView.LookAt(_cameraLookAtPosition);
            SceneView.lastActiveSceneView.Repaint();
#endif // if UNITY_EDITOR
        }

        public void SetCamera(Vector3 cameraPosition, Vector3 cameraLookAtPosition)
        {
            _cameraPosition = cameraPosition;
            _cameraLookAtPosition = cameraLookAtPosition;
        }
    }
}
