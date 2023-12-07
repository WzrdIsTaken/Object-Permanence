using System.Collections.Generic;
using System.Linq;
using UnityEngine;
using UnityEngine.InputSystem;
using UnityEngine.XR.Interaction.Toolkit;

namespace ObjectPermanence
{
    /**
     * When player is holding an item, we don't want the player to rotate as well
     */
    public class VRStopMovementRotationWhenHoldingItem : MonoBehaviour
    {
        [System.Serializable]
        struct ControllerInfo
        {
            public InputActionProperty OverrideButton;
            public XRRayInteractor RayInteractor;

            public InputActionProperty SelectButton; // because now the events dont work??
            [HideInInspector] public bool HoldingItem;
            [HideInInspector] public GameObject HeldItem;
            [HideInInspector] public Quaternion HeldItemRot;

            public bool IsHoldingItem() => HoldingItem; //RayInteractor.interactablesSelected.Count > 0; I can't seem to use this? Have to do the event stuff instead
            public bool IsOverrideButtonPressed() => OverrideButton.action.IsPressed();
            public bool IsSelectButtonPressed() => SelectButton.action.IsPressed();
        }

        [Header("Movement Providers")]
        [SerializeField] SnapTurnProviderBase _snapTurnProvider;
        [SerializeField] ContinuousTurnProviderBase _continuousTurnProvider;
        [SerializeField] TeleportationProvider _teleportationProvider;
        [SerializeField] ContinuousMoveProviderBase _continousMoveProvider;

        bool _snapTurnProviderDefaultState;
        bool _continuousTurnDefaultState;
        float _continousTurnDefaultTurnSpeed;
        bool _teleportationProviderDefaultState;
        bool _continousMoveProviderDefaultState;
        float _teleportationProviderDefaultDelay;
        float _continousMoveProviderDefaultSpeed;

        bool turnComponentsDisabled = false;
        bool moveComponentsDisabled = false;

        [Header("Controllers")]
        [SerializeField] ControllerInfo _leftController;
        [SerializeField] ControllerInfo _rightController;

        // --- Main Loop ---

        void Start()
        {
            SetProviderDefaultStates();
        }

        void Update()
        {
            ToggleOffHoldingState(_leftController);
            ToggleMovementProviders(ref _leftController);

            ToggleOffHoldingState(_rightController);
            ToggleMovementProviders(ref _rightController);
        }

        void LateUpdate()
        {
            if (_leftController.IsHoldingItem() && !_leftController.IsOverrideButtonPressed())
            {
                _leftController.HeldItem.transform.rotation = _leftController.HeldItemRot;
            }
            if (_rightController.IsHoldingItem() && !_rightController.IsOverrideButtonPressed())
            {
                _rightController.HeldItem.transform.rotation = _rightController.HeldItemRot;
            }
        }

        void ToggleMovementProviders(ref ControllerInfo controllerInfo)
        {
            bool holdingItem = controllerInfo.IsHoldingItem();
            bool overrideButtonPressed = controllerInfo.IsOverrideButtonPressed();

            if (holdingItem && overrideButtonPressed)
            {
                if (controllerInfo.RayInteractor == _rightController.RayInteractor)
                {
                    ToggleTurnComponentStates(false);
                }
                if (controllerInfo.RayInteractor == _leftController.RayInteractor)
                {
                    ToggleMoveComponentsStates(false);
                }
                
                return;
            }
            else
            {
                if (controllerInfo.RayInteractor == _rightController.RayInteractor)
                {
                    ToggleTurnComponentStates(true);
                }
                if (controllerInfo.RayInteractor == _leftController.RayInteractor)
                {
                    ToggleMoveComponentsStates(true);
                }
            }

            if (overrideButtonPressed && holdingItem)
            {
                controllerInfo.HeldItemRot = controllerInfo.HeldItem.transform.rotation;
            }
            //ToggleHeldItem(controllerInfo, overrideButtonPressed);
        }

        void ToggleOffHoldingState(ControllerInfo controllerInfo)
        {
            if (controllerInfo.IsHoldingItem())
            {
                if (!controllerInfo.IsSelectButtonPressed())
                {
                    ToggleRayInteractorHoldingItemStates(controllerInfo.RayInteractor, false);
                }
            }
        }

        // --- Events ---

        public void OnRayInteractorSelected(XRRayInteractor rayInteractor) 
        {
            ToggleRayInteractorHoldingItemStates(rayInteractor, true);
        }

        public void OnRayInteractorDeselected(XRRayInteractor rayInteractor)
        {
            // This event doesn't seem to work??
            //ToggleRayInteractorHoldingItemStates(rayInteractor, false);
        }

        private void ToggleRayInteractorHoldingItemStates(XRRayInteractor rayInteractor, bool state)
        {
            if (rayInteractor == _leftController.RayInteractor) 
            {
                _leftController.HoldingItem = state;
                DebugManager.Instance.Log(LogLevel.Info, DebugCategory.VR, $"Left ray interactor holding state {state}");

                if (!state)
                {
                    _leftController.HeldItem = null;
                }
                else
                {
                    SetHeldItem(ref _leftController);
                }
            }
            if (rayInteractor == _rightController.RayInteractor) 
            {
                _rightController.HoldingItem = state;
                DebugManager.Instance.Log(LogLevel.Info, DebugCategory.VR, $"Right ray interactor holding state {state}");

                if (!state)
                {
                    _rightController.HeldItem = null;
                }
                else
                {
                    SetHeldItem(ref _rightController);
                }
            }
        }

        // --- Helper Funcs ---

        public void SetProviderDefaultStates()
        {
            // Will need to call this in settings update
            _snapTurnProviderDefaultState = _snapTurnProvider.enabled;
            _continuousTurnDefaultState = _continuousTurnProvider.enabled;
            _continousTurnDefaultTurnSpeed = _continuousTurnProvider.turnSpeed;

            _teleportationProviderDefaultState = _teleportationProvider.enabled;
            _teleportationProviderDefaultDelay = _teleportationProvider.delayTime;
            _continousMoveProviderDefaultState = _continousMoveProvider.enabled;
            _continousMoveProviderDefaultSpeed = _continousMoveProvider.moveSpeed;
        }

        void ToggleMoveComponentsStates(bool state)
        {
            if (state)
            {
                if (!moveComponentsDisabled)
                {
                    if (_teleportationProviderDefaultState)
                    {
                        _teleportationProvider.delayTime = _teleportationProviderDefaultDelay;
                    }
                    if (_continousMoveProviderDefaultState)
                    {
                        _continousMoveProvider.moveSpeed = _continousMoveProviderDefaultSpeed;
                    }
                }
            }
            else
            {
                if (moveComponentsDisabled)
                {
                    _teleportationProvider.delayTime = float.PositiveInfinity;
                    _continousMoveProvider.moveSpeed = 0.0f;
                }
            }

            moveComponentsDisabled = state;
        }

        void ToggleTurnComponentStates(bool state)
        {
            if (state)
            {
                if (!turnComponentsDisabled)
                {
                    if (_snapTurnProviderDefaultState)
                    {
                        _snapTurnProvider.enableTurnLeftRight = true;
                        _snapTurnProvider.enableTurnAround = true;
                    }
                    if (_continuousTurnDefaultState)
                    {
                        _continuousTurnProvider.turnSpeed = _continousTurnDefaultTurnSpeed;
                    }
                }
            }
            else
            {
                if (turnComponentsDisabled)
                {
                    _snapTurnProvider.enableTurnLeftRight = false;
                    _snapTurnProvider.enableTurnAround = false;
                    _continuousTurnProvider.turnSpeed = 0.0f;
                }
            }

            turnComponentsDisabled = state;
        }

        void SetHeldItem(ref ControllerInfo controllerInfo)
        {
            List<GameObject> nearbyValidItems = GameObject.FindGameObjectsWithTag(Tags.CollectableTag).ToList();
            nearbyValidItems.AddRange(GameObject.FindGameObjectsWithTag(Tags.PickupableTag).ToList());

            if (nearbyValidItems.Count > 0)
            {
                Vector3 playerPos = transform.position;
                nearbyValidItems.Sort((i1, i2) =>
                {
                    float i1Dis = Vector3.Distance(i1.transform.position, playerPos);
                    float i2Dis = Vector3.Distance(i2.transform.position, playerPos);
                    return i1Dis.CompareTo(i2Dis);
                });

                controllerInfo.HeldItem = nearbyValidItems[0];
                controllerInfo.HeldItemRot = controllerInfo.HeldItem.transform.rotation;
                DebugManager.Instance.Log(LogLevel.Info, DebugCategory.VR, $"Setting {controllerInfo.RayInteractor.gameObject.name} held item to {controllerInfo.HeldItem.name}");
            }
        }

        void ToggleHeldItem(ControllerInfo controllerInfo, bool state)
        {
            if (controllerInfo.HoldingItem)
            {
                controllerInfo.HeldItem.GetComponent<Rigidbody>().constraints = state ? RigidbodyConstraints.FreezeRotation : RigidbodyConstraints.None;
            }
        }
    }
}
