using System.Collections;
using System.Collections.Generic;
using UnityEngine;

namespace ObjectPermanence
{
    /**
     * Routes through the AudioManager to play music on a loop
     * Note - the source voice returned from PlayMusicGroup will remain the same for all tracks 
     */

    public struct MusicPlaySettings
    {
        public static MusicPlaySettings Default = new(true, false);

        public bool ShuffleTracks;
        public bool FadeSongs;

        public MusicPlaySettings(bool shuffleTracks, bool fadeSongs)
        {
            ShuffleTracks = shuffleTracks;
            FadeSongs = fadeSongs;
        }
    }

    public class MusicManager : Singleton<MusicManager>
    {
        private Dictionary<AudioGroupID, AudioEffectGroup> _musicGroups;
        private AudioGroupID _currentMusicGroupID;
        private List<AudioID> _currentMusicGroupPlayedIds;
        private AudioSource _currentMusicSourceVoice;

        private const string _musicGroupsResourcePath = "Audio/MusicGroups";

        private MusicManager()
        {
            _musicGroups = new Dictionary<AudioGroupID, AudioEffectGroup>();
            _currentMusicGroupID = AudioGroupID.None;
            _currentMusicGroupPlayedIds = new List<AudioID>();
            _currentMusicSourceVoice = null;
        }

        protected override void Awake()
        {
            base.Awake();
            InitMusicGroups(_musicGroupsResourcePath);
        }

        public AudioSource PlayMusicGroup(AudioGroupID id, MusicPlaySettings playSettings)
        {
            DebugManager.Instance.ConditionalLog(_currentMusicGroupID != AudioGroupID.None, LogLevel.Warning, DebugCategory.Audio,
                "Playing another music group but the current one hasn't been stopped yet. Consider calling StopCurrentMusicGroup?");
            return PlayMusicGroupInternal(id, playSettings);
        }

        public void StopCurrentMusicGroup()
        {
            StopAllCoroutines();

            _currentMusicGroupID = AudioGroupID.None;
            _currentMusicGroupPlayedIds.Clear();
            AudioManager.Instance.StopVoice(_currentMusicSourceVoice);
            _currentMusicSourceVoice = null;
        }

        private void InitMusicGroups(string path)
        {
            // Thoughts on combining this with AudioManager's InitAudioEffects music somehow?
            // Pros: they are pretty similar. Cons: reduces clarity

            AudioEffectGroup[] audioEffectGroups = Resources.LoadAll<AudioEffectGroup>(path);
            foreach (AudioEffectGroup audioEffectGroup in audioEffectGroups)
            {
                DebugManager.Instance.Assert(audioEffectGroup.ID != AudioGroupID.None, AssertLevel.Assert, DebugCategory.Audio,
                       $"Loading an music group \"{audioEffectGroup.name}\" which doesn't have a valid ID");
                DebugManager.Instance.Assert(audioEffectGroup.Effects.Count != 0, AssertLevel.Assert, DebugCategory.Audio,
                        $"Loading an music group \"{audioEffectGroup.name}\" which doesn't have any tracks");

                if (!_musicGroups.ContainsKey(audioEffectGroup.ID))
                {
                    _musicGroups.Add(audioEffectGroup.ID, audioEffectGroup);

                    DebugManager.Instance.Log(LogLevel.Info, DebugCategory.Audio,
                        $"Loaded a music group with ID \"{audioEffectGroup.ID}\". Music groups loaded: {_musicGroups.Count}");
                }
                else
                {
                    DebugManager.Instance.AssertFail(AssertLevel.Assert, DebugCategory.Audio,
                        $"Trying to load a music group \"{audioEffectGroup.name}\" with ID \"{audioEffectGroup.ID}\" which already exists");
                }
            }
        }

        private AudioSource PlayMusicGroupInternal(AudioGroupID id, MusicPlaySettings playSettings)
        {
            if (_musicGroups.ContainsKey(id))
            {
                _currentMusicGroupID = id;
                AudioID musicID = AudioID.None;

                if (_musicGroups[id].Effects.Count == _currentMusicGroupPlayedIds.Count)
                {
                    _currentMusicGroupPlayedIds.Clear();
                }

                if (playSettings.ShuffleTracks)
                {
                    _musicGroups[id].Effects.Shuffle();
                }

                if (playSettings.FadeSongs)
                {
                    DebugManager.Instance.AssertFail(AssertLevel.Soft, DebugCategory.Audio,
                        "Song fading currently isn't implemented yet. Will it ever be?");
                    // I've done it before, see https://github.com/TopLads/wardens-teddy/blob/main/Wardens%20Teddy/Assets/Scripts/Managers/MusicManager.cs
                }

                foreach (AudioID audioID in _musicGroups[id].Effects)
                {
                    if (!_currentMusicGroupPlayedIds.Contains(audioID))
                    {
                        musicID = audioID;
                        break;
                    }
                }

                _currentMusicSourceVoice = AudioManager.Instance.PlayEffect(musicID, AudioMixerID.Music, AudioPlaySettings.Default, _currentMusicSourceVoice);
                StartCoroutine(PlayNextSong(_currentMusicSourceVoice.clip.length, playSettings));
            }
            else
            {
                DebugManager.Instance.AssertFail(AssertLevel.Assert, DebugCategory.Audio,
                    $"Trying to play a music group with id \"{id}\" that doesn't exist");
            }

            return _currentMusicSourceVoice;
        }

        private IEnumerator PlayNextSong(float currentSongLength, MusicPlaySettings playSettings)
        {
            yield return new WaitForSeconds(currentSongLength);
            PlayMusicGroupInternal(_currentMusicGroupID, playSettings);
        }
    }
}
