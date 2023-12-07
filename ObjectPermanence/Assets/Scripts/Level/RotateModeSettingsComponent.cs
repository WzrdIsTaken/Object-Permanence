using TMPro;
using UnityEngine;
using UnityEngine.XR.Interaction.Toolkit;
using UnityEngine.XR.Interaction.Toolkit.Inputs;

namespace ObjectPermanence
{
    /**
     * For the settings menu
     */
    public class RotateModeSettingsComponent : MonoBehaviour
    {
        enum RotateMode
        {
            Continuous,
            Snap
        }

        [SerializeField] ContinuousTurnProviderBase _playerContinousTurnProvider;
        [SerializeField] SnapTurnProviderBase _playerSnapTurnProvider;

        [SerializeField] TMP_Text _buttonContent;
        RotateMode _rotateMode = RotateMode.Snap;

        bool _acceptInput = true; // ???

        void Start()
        {
            SetButtonText();
            SetPlayerTurnProviders();
        }

        public void ButtonPress()
        {
            if (!_acceptInput) return;

            _rotateMode = _rotateMode.Next();
            SetButtonText();
            SetPlayerTurnProviders();

            _acceptInput = false;
            Invoke(nameof(SetAcceptInput), 0.1f);
        }

        void SetButtonText()
        {
            _buttonContent.text = _rotateMode.ToString() + " Rotate";
        }

        void SetPlayerTurnProviders()
        {
            // got an odd issue where this fails if it happens more than once?? idk dude, it will be fine for video though

            switch (_rotateMode)
            {
                case RotateMode.Continuous:
                    _playerSnapTurnProvider.enabled = false;
                    _playerContinousTurnProvider.enabled = true;
                    break;
                case RotateMode.Snap:
                    _playerContinousTurnProvider.enabled = false;
                    _playerSnapTurnProvider.enabled = true;
                    break;
                default:
                    DebugManager.Instance.AssertFail(AssertLevel.Assert, DebugCategory.Level, $"Unrecognised {nameof(RotateMode)}");
                    break;
            }

            _playerContinousTurnProvider.gameObject.GetComponent<VRStopMovementRotationWhenHoldingItem>().SetProviderDefaultStates();
        }

        void SetAcceptInput()
        {
            _acceptInput = true;
        }
    }
}
