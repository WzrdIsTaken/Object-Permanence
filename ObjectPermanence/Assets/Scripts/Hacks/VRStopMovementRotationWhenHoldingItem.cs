using UnityEngine;
using UnityEngine.InputSystem;
using UnityEngine.XR.Interaction.Toolkit;

namespace ObjectPermanence
{
    /**
     * When player is holding an item, we don't want to rotate as well
     */
    public class VRStopMovementRotationWhenHoldingItem : MonoBehaviour
    {
        // its kinda nice writing bot code

        [SerializeField] SnapTurnProviderBase snapTurn;
        [SerializeField] ContinuousTurnProviderBase contTurn;
        [SerializeField] InputActionProperty _override;
        [SerializeField] InputActionProperty _override2;
        [SerializeField] XRRayInteractor rightDirectGrab;
        [SerializeField] XRRayInteractor leftDirectGrab;

        private float startRotateSpeed;

        private void Start()
        {
            startRotateSpeed = contTurn.turnSpeed;
        }

        private void Update()
        {
            bool check = true;
            bool holdingRight = rightDirectGrab.interactablesSelected.Count > 0;
            bool holdingLeft = leftDirectGrab.interactablesSelected.Count > 0;

            if (holdingRight || holdingLeft)
            {
                if (_override.action.IsPressed() || _override2.action.IsPressed())
                {
                    snapTurn.enableTurnLeftRight = false;
                    snapTurn.enableTurnAround = false;
                    contTurn.turnSpeed = 0.0f;
                    check = false;
                }
                else
                {
                    //Freeze(leftDirectGrab, RigidbodyConstraints.FreezeRotation);
                    //Freeze(rightDirectGrab, RigidbodyConstraints.FreezeRotation);
                }
            }
            
            if (check)
            {
                snapTurn.enableTurnLeftRight = true;
                snapTurn.enableTurnAround = true;
                contTurn.turnSpeed = startRotateSpeed;

                //Freeze(leftDirectGrab, RigidbodyConstraints.None);
                //Freeze(rightDirectGrab, RigidbodyConstraints.None);
            }
        }

        private void Freeze(XRRayInteractor grab, RigidbodyConstraints con)
        {
            // hoesntly its fine
            // will make note 
            // can fix if want
            return;
            
            /*
            foreach (var v in grab.interactablesSelected)
            {
                Rigidbody r = v.transform.gameObject.GetComponent<Rigidbody>() ?? v.transform.gameObject.GetComponentInParent<Rigidbody>();
                if (r)
                {
                    r.constraints = con;
                }
            }
            */
        }

        /*
        [SerializeField] private InputActionProperty _toggleOffAction;
        [SerializeField] private ContinuousTurnProviderBase _continuousMoveProvider;
        [SerializeField] private SnapTurnProviderBase _snapTurnMoveProvider;

        private bool _continiousTurnProviderDefaultState;
        private bool _snapTurnProviderDefaultState;
        private bool _off;

        public VRStopMovementRotationWhenHoldingItem()
        {
            _toggleOffAction = default;
            _continuousMoveProvider = null;
            _snapTurnMoveProvider = null;

            _continiousTurnProviderDefaultState = false;
            _snapTurnProviderDefaultState = false;
            _off = false;
        }

        void Start()
        {
            RefreshComponentDefaultState();
        }

        void Update()
        {
            if (!_off && _toggleOffAction.action.ReadValue<float>() > 0.75f)
            {
                _continuousMoveProvider.enabled = false;
                _snapTurnMoveProvider.enabled = false;
                _off = true;
            }
            else if (_off)
            {
                _continuousMoveProvider.enabled = _continiousTurnProviderDefaultState;
                _snapTurnMoveProvider.enabled = _snapTurnProviderDefaultState;
                _off = false;
            }
        }

        public void RefreshComponentDefaultState()
        {
            _continiousTurnProviderDefaultState = _continuousMoveProvider.enabled;
            _snapTurnProviderDefaultState = _snapTurnMoveProvider.enabled;
        }
        */
    }
}
