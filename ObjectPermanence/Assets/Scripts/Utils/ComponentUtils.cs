using UnityEngine;

namespace ObjectPermanence
{
    /**
     * (right now only) Contains toggle methods for components. Different component's have
     * there "enabled" property at different points in there inheritance trees, and some
     * components (eg rigidbody) need to be toggled in different ways.
     * 
     * Some components just can't be disabled in any way, but we should override them anyway
     * to show intent. This will also mean the the assert in the first ToggleComponent definition 
     * fires only when needed.
     */
    public static class ComponentUtils
    {
        // Base types

#pragma warning disable IDE0060 // Remove unused parameter
        public static void ToggleComponent(Component component, bool toggleState)
#pragma warning restore IDE0060 // Remove unused parameter
        {
            // To fix this assert, just add a new overriden method for the component type
            DebugManager.Instance.AssertFail(AssertLevel.Assert, DebugCategory.Level,
                $"Tried to toggle the component \"{component.GetType()}\" on \"{component.gameObject.name}\", but " +
                $"there is no overriden ToggleComponent method for its type");
        }

        public static void ToggleComponent(Behaviour behaviour, bool toggleState)
        {
            behaviour.enabled = toggleState;
        }

        // Alphabetical from here

        public static void ToggleComponent(Collider collder, bool toggleState)
        {
            collder.enabled = toggleState;
        }

#pragma warning disable IDE0060 // Remove unused parameter
        public static void ToggleComponent(MeshFilter meshFilter, bool toggleState)
#pragma warning restore IDE0060 // Remove unused parameter
        {
        }

        public static void ToggleComponent(Renderer renderer, bool toggleState)
        {
            renderer.enabled = toggleState;
        }

        public static void ToggleComponent(Rigidbody rigidbody, bool toggleState)
        {
            rigidbody.isKinematic = !toggleState;
            rigidbody.useGravity = toggleState;
        }

#pragma warning disable IDE0060 // Remove unused parameter
        public static void ToggleComponent(Transform transform, bool toggleState)
#pragma warning restore IDE0060 // Remove unused parameter
        {

        }
    }
}
