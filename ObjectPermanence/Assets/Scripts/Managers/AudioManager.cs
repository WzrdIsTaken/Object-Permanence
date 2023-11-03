using System;
using System.Collections.Generic;
using System.Linq;
using UnityEngine;
using UnityEngine.Audio;

namespace ObjectPermanence
{
    /**
     * All audio is routed through this class.
     */

    public struct AudioPlaySettings
    {
        public static AudioPlaySettings Default = new(1.0f, 1.0f, false);

        public float Volume;
        public float Pitch;
        public bool Loop;

        public AudioPlaySettings(float volume, float pitch, bool loop)
        {
            Volume = volume;
            Pitch = pitch;
            Loop = loop;
        }
    }

    public enum AudioMixerID : uint
    {
        Music,
        SFX
    };

    public class AudioManager : Singleton<AudioManager>
    {
        [SerializeField] private AudioMixer _mainMixer;
        [SerializeField] private uint _sourceVoicePoolSize;

        private Dictionary<AudioMixerID, AudioMixerGroup> _audioMixers;
        private Dictionary<AudioID, AudioEffect> _audioEffects;
        private AudioSource[] _sourceVoicePool;

        private const string _audioEffectsResourcePath = "Audio/Effects";
        private const string _musicMixerName = "Music";
        private const string _sfxMixerName = "SFX";

        private AudioManager()
        {
            _mainMixer = null;
            _sourceVoicePoolSize = 3u;

            _audioMixers = new Dictionary<AudioMixerID, AudioMixerGroup>();
            _audioEffects = new Dictionary<AudioID, AudioEffect>();
            _sourceVoicePool = null;
        }

        protected override void Awake()
        {
            base.Awake();

            InitAudioMixers(_musicMixerName, _sfxMixerName);
            InitSourceVoicePool(_sourceVoicePoolSize);
            InitAudioEffects(_audioEffectsResourcePath);
        }

        public AudioSource PlayEffect(AudioID audioID, AudioMixerID mixerID, AudioPlaySettings playSettings, AudioSource existingSource = null)
        {
            AudioMixerGroup mixer = GetAudioMixer(mixerID);
            AudioEffect effect = GetAudioEffect(audioID);
            AudioSource voice = existingSource ? existingSource : GetAudioSourceVoice();

            if (mixer && effect && voice)
            {
                SetAudioSourceForEffect(voice, mixer, effect, playSettings);
                voice.Play();
            }

            return voice;
        }

        public void StopAllAudioEffects(AudioID id)
        {
            AudioEffect effect = GetAudioEffect(id);
            StopAllAudioEffectsHelper(effect, (v => v.clip == effect.Clip));
        }

        public void StopAllAudioEffects(AudioMixerID id)
        {
            AudioMixerGroup mixer = GetAudioMixer(id);
            StopAllAudioEffectsHelper(mixer, (v => v.outputAudioMixerGroup == mixer));
        }

        public AudioMixerGroup GetAudioMixer(AudioMixerID id)
        {
            return GetAudioHelper(id, _audioMixers);
        }

        public AudioEffect GetAudioEffect(AudioID id)
        {
            return GetAudioHelper(id, _audioEffects);
        }

        private void InitSourceVoicePool(uint poolSize)
        {
            _sourceVoicePool = new AudioSource[poolSize];

            for (uint i = 0u; i < poolSize; ++i)
            {
                GameObject sourceVoice = new($"SourceVoice{i}");
                sourceVoice.transform.parent = transform;
                _sourceVoicePool[i] = sourceVoice.AddComponent<AudioSource>();
            }
        }

        private void InitAudioEffects(string path)
        {
            AudioEffect[] audioEffects = Resources.LoadAll<AudioEffect>(path);
            foreach (AudioEffect audioEffect in audioEffects)
            {
                DebugManager.Instance.Assert(audioEffect.ID != AudioID.None, AssertLevel.Assert, DebugCategory.Audio,
                        $"Loading an audio effect \"{audioEffect.name}\" which doesn't have a valid ID");
                DebugManager.Instance.Assert(audioEffect.Clip, AssertLevel.Assert, DebugCategory.Audio,
                        $"Loading an audio effect \"{audioEffect.name}\" which doesn't have a valid clip");

                if (!_audioEffects.ContainsKey(audioEffect.ID))
                {
                    _audioEffects.Add(audioEffect.ID, audioEffect);

                    DebugManager.Instance.Log(LogLevel.Info, DebugCategory.Audio,
                        $"Loaded audio effect with ID \"{audioEffect.ID}\". Audio effects loaded: {_audioEffects.Count}");
                }
                else
                {
                    DebugManager.Instance.AssertFail(AssertLevel.Assert, DebugCategory.Audio,
                        $"Trying to load an audio effect \"{audioEffect.name}\" with ID \"{audioEffect.ID}\" which already exists");
                }
            }
        }

        private void InitAudioMixers(string musicMixerName, string sfxMixerName)
        {
            void InitAudioMixersHelper(AudioMixerID id, string mixerName)
            {
                AudioMixerGroup mixer = _mainMixer.FindMatchingGroups(mixerName).First();
                if (mixer)
                {
                    _audioMixers.Add(id, mixer);
                }
                else
                {
                    DebugManager.Instance.AssertFail(AssertLevel.Assert, DebugCategory.Audio,
                        $"Couldn't find a music mixer with name \"{mixerName}\"");
                }
            }

            InitAudioMixersHelper(AudioMixerID.Music, musicMixerName);
            InitAudioMixersHelper(AudioMixerID.SFX, sfxMixerName);
        }

        private AudioSource GetAudioSourceVoice()
        {
            AudioSource source = null;
            source = _sourceVoicePool.Where(v => !v.isPlaying).FirstOrDefault();

            DebugManager.Instance.Assert(source, AssertLevel.Assert, DebugCategory.Audio,
                $"Couldn't find an available source voice. Consider increasing the pool size?");
            return source;
        }

        private void SetAudioSourceForEffect(AudioSource voice, AudioMixerGroup mixer, AudioEffect effect, AudioPlaySettings playSettings)
        {
            voice.outputAudioMixerGroup = mixer;
            voice.clip = effect.Clip;

            voice.volume = playSettings.Volume;
            voice.pitch = playSettings.Pitch;
            voice.loop = playSettings.Loop;
        }

        private void StopAllAudioEffectsHelper<T>(T comparisonType, Func<AudioSource, bool> findFunc) where T : class
        {
            if (comparisonType != null)
            {
                IEnumerable<AudioSource> voices = _sourceVoicePool.Where(findFunc);
                foreach (AudioSource voice in voices)
                {
                    voice.Stop();
                }
            }
        }

        private U GetAudioHelper<T, U>(T id, Dictionary<T, U> dictionary) where T : Enum /*,*/ where U : class
        {
            U idType = null;

            if (dictionary.ContainsKey(id))
            {
                idType = dictionary[id];
            }
            else
            {
                DebugManager.Instance.AssertFail(AssertLevel.Assert, DebugCategory.Audio,
                    $"Couldn't find an audio type with ID \"{id}\"");
            }

            return idType;
        }
    }
}
