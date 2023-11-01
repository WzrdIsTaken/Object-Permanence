using UnityEngine;

namespace ObjectPermanence
{
    /**
     * A collection of functions and extension methods for manipulating transforms.
     */
    public static class TransformUtils
    {
        public static Transform DestroyChildren(this Transform transform) // good function name
        {
            foreach (Transform child in transform)
            {
                Object.Destroy(child);
            }

            return transform;
        }

        public static void SineWaveBob(GameObject obj, Transform anchor, float wobblePosSpeed, float wobblePosAmount, float wobbleRotSpeed, float wobbleRotAmount)
        {
            float addToPos = Mathf.Sin(Time.time * wobblePosSpeed) * wobblePosAmount;
            obj.transform.localPosition += addToPos * Time.deltaTime * Vector3.up;

            float xRot = Mathf.Sin(Time.time * wobbleRotSpeed) * wobbleRotAmount;
            float zRot = Mathf.Sin((Time.time - 1f) * wobbleRotSpeed) * wobbleRotAmount;

            obj.transform.localEulerAngles = new Vector3(xRot, anchor.eulerAngles.y, zRot);
        }
    }
}
