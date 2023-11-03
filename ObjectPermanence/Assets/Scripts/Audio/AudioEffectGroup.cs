using System.Collections.Generic;
using UnityEngine;

namespace ObjectPermanence
{
    /**
     * A group of audio effects.
     */

    [CreateAssetMenu(fileName = "AudioEffectGroup", menuName = "Audio/AudioEffectGroup", order = 2)]
    public class AudioEffectGroup : ScriptableObject
    {
        public AudioGroupID ID = AudioGroupID.None;
        public List<AudioID> Effects = new();
    }
}
