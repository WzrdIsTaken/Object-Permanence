using System;
using UnityEngine;
using UnityEngine.XR.Interaction.Toolkit;

namespace ObjectPermanence
{
    /**
     * Contains logic for things like making the controller buzz. This is used when eg the player gets game ended.
     */
    public static class VRUtils
    {
        public static void ForeachController(Action<XRBaseController> action)
        {
            XRBaseController[] controllers = GameObject.FindObjectsOfType<XRBaseController>();
            if (controllers.Length != 0)
            {
                foreach (XRBaseController controller in controllers)
                {
                    action(controller);
                }
            }
        }

        public static void TriggerHaptic(XRBaseController controller, float strength, float duration)
        {
            strength = Mathf.Clamp01(strength);
            if (strength > 0.0f && duration > 0.0f)
            {
                DebugManager.Instance.Log(LogLevel.Info, DebugCategory.VR, 
                    $"Trigging haptic feedback on controller \"{controller.name}\". Strength: {strength} Duration: {duration}");
                controller.SendHapticImpulse(strength, duration);
            }
        }
    }
}
