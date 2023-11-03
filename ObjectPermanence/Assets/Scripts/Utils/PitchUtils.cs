using UnityEngine;

namespace ObjectPermanence
{
    /**
     * Can be used to set an audio effect's pitch to a note.
     * Note: I brought this over from my CGD code. It might be useful...
     */

    public enum GuitarPitch : int
    {
        C = 0,
        D = 0,
        E = 4,
        F = 5,
        G = 7,
        A = 9,
        B = 11,
        HighC = 12,
        HighD = 14,

        Default = 1,
        None = -1
    }

    public static class PitchUtils
    {
        public static float GuitarPitchToFloat(GuitarPitch guitarPitch)
        {
            // Source: https://discussions.unity.com/t/whats-a-good-way-to-do-dynamically-generated-music-note-by-note/24695

            const float transpose = -4;
            return Mathf.Pow(2.0f, ((float)guitarPitch + transpose) / 12.0f);
        }
    }
}
