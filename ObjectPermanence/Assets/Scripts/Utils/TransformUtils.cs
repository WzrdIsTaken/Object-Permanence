using UnityEngine;

namespace ObjectPermanence
{
    /**
     * A collection of functions and extension methods for manipulating transforms.
     */
    public static class TransformUtils
    {
        public static Transform DestroyChildren(this Transform transform)
        {
            foreach (Transform child in transform)
            {
                Object.Destroy(child);
            }

            return transform;
        }
    }
}
