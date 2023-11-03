using UnityEngine;

namespace ObjectPermanence
{
    /**
     * Audio data. Place in Resources->Audio->Effects.
     */
    [CreateAssetMenu(fileName = "AudioEffect", menuName = "Audio/AudioEffect", order = 1)]
    public class AudioEffect : ScriptableObject
    {
        public AudioID ID = AudioID.None;
        public AudioClip Clip = null;
    }
}
