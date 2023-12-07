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
        public static AudioPlaySettings Default = new(1.0f, 1.0f, false, null);

        public float Volume;
        public float Pitch;
        public bool Loop;
        public Vector3? Position;

        public AudioPlaySettings(float volume, float pitch, bool loop, Vector3? position)
        {
            Volume = volume;
            Pitch = pitch;
            Loop = loop;
            Position = position;
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
        [SerializeField] private uint _voicePoolSize;

        private Dictionary<AudioMixerID, AudioMixerGroup> _audioMixers;
        private Dictionary<AudioID, AudioEffect> _audioEffects;
        private AudioSource[] _voicePool;

        private const string _audioEffectsResourcePath = "Audio/Effects";
        private const string _musicMixerName = "Music";
        private const string _sfxMixerName = "SFX";

        private AudioManager()
        {
            _mainMixer = null;
            _voicePoolSize = 3u;

            _audioMixers = new Dictionary<AudioMixerID, AudioMixerGroup>();
            _audioEffects = new Dictionary<AudioID, AudioEffect>();
            _voicePool = null;
        }

        protected override void Awake()
        {
            base.Awake();

            InitAudioMixers(_musicMixerName, _sfxMixerName);
            InitVoicePool(_voicePoolSize);
            InitAudioEffects(_audioEffectsResourcePath);
        }

        public AudioSource PlayEffect(AudioID audioID, AudioMixerID mixerID, AudioPlaySettings playSettings, AudioSource existingSource = null)
        {
            AudioMixerGroup mixer = GetMixer(mixerID);
            AudioEffect effect = GetEffect(audioID);
            AudioSource voice = existingSource ? existingSource : GetAvailableVoice();

            if (mixer && effect && voice)
            {
                DebugManager.Instance.Log(LogLevel.Info, DebugCategory.Audio, $"Playing audio effect {audioID}");

                SetAudioVoiceForEffect(voice, mixer, effect, playSettings);
                voice.Play();
            }

            return voice;
        }

        public void StopVoice(AudioSource voice)
        {
            if (voice)
            {
                voice.Stop();
            }
        }

        public void StopVoices(IEnumerable<AudioSource> voices)
        {
            foreach (AudioSource voice in voices)
            {
                StopVoice(voice);
            }
        }

        public IEnumerable<AudioSource> GetAllVoices(AudioID id)
        {
            AudioEffect effect = GetEffect(id);
            return GetAudioVoicesHelper(effect, (v => v.clip == effect.Clip));
        }

        public IEnumerable<AudioSource> GetAllVoices(AudioMixerID id)
        {
            AudioMixerGroup mixer = GetMixer(id);
            return GetAudioVoicesHelper(mixer, (v => v.outputAudioMixerGroup == mixer));
        }

        public AudioMixerGroup GetMixer(AudioMixerID id)
        {
            return GetAudioAssetHelper(id, _audioMixers);
        }

        public AudioEffect GetEffect(AudioID id)
        {
            return GetAudioAssetHelper(id, _audioEffects);
        }

        private void InitVoicePool(uint poolSize)
        {
            _voicePool = new AudioSource[poolSize];

            for (uint i = 0u; i < poolSize; ++i)
            {
                var voice = new GameObject($"Voice{i}");
                voice.transform.parent = transform;
                _voicePool[i] = voice.AddComponent<AudioSource>();
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
                AudioMixerGroup mixer = _mainMixer.FindMatchingGroups(mixerName).FirstOrDefault();
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

        private AudioSource GetAvailableVoice()
        {
            AudioSource voice = null;
            voice = _voicePool.Where(v => !v.isPlaying).FirstOrDefault();

            DebugManager.Instance.Assert(voice, AssertLevel.Assert, DebugCategory.Audio,
                $"Couldn't find an available voice. Consider increasing the pool size?");
            return voice;
        }

        private void SetAudioVoiceForEffect(AudioSource voice, AudioMixerGroup mixer, AudioEffect effect, AudioPlaySettings playSettings)
        {
            voice.outputAudioMixerGroup = mixer;
            voice.clip = effect.Clip;

            voice.volume = playSettings.Volume;
            voice.pitch = playSettings.Pitch;
            voice.loop = playSettings.Loop;

            if (playSettings.Position.HasValue)
            {
                const float spatialBlend3D = 1.0f;
                voice.spatialBlend = spatialBlend3D;
                voice.transform.position = playSettings.Position.Value;
            }
            else
            {
                const float spatialBlend2D = 0.0f;
                voice.spatialBlend = spatialBlend2D;
            }
        }

        private IEnumerable<AudioSource> GetAudioVoicesHelper<T>(T comparisonType, Func<AudioSource, bool> findFunc) where T : class
        {
            IEnumerable<AudioSource> voices = null;

            if (comparisonType != null)
            {
                voices = _voicePool.Where(findFunc);
            }

            return voices;
        }

        private U GetAudioAssetHelper<T, U>(T id, Dictionary<T, U> dictionary) where T : Enum /*,*/ where U : class
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
