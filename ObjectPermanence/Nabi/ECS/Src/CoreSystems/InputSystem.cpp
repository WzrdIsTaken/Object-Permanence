#include "Core.h"

#include "CoreSystems/InputSystem.h"

#include "InputIncludes.h"

#include "Context.h"
#include "CoreModules\InputModule.h"
#include "GameTime.h"
#include "InputCodes.h"

namespace ecs
{
	REFLECT_SYSTEM_BEGIN_DEFAULT(InputSystem)
	RELFECT_SYSTEM_END(InputSystem)

	using namespace nabi::Input;

	InputSystem::InputSystem(nabi::Context& context, entt::hashed_string const systemId, entt::hashed_string const systemGroupId)
		: SystemBase(context, systemId, systemGroupId)
	{
		REGISTER_SYSTEM_UPDATE_EVENT_SUBSCRIBER(InputSystem)

		// Keyboard 
		REGISTER_WINDOWS_EVENT_SUBSCRIBER(WM_KEYDOWN,     InputSystem::OnKeyboardKeyDown);
		REGISTER_WINDOWS_EVENT_SUBSCRIBER(WM_KEYUP,       InputSystem::OnKeyboardKeyUp);

		// Mouse
		REGISTER_WINDOWS_EVENT_SUBSCRIBER(WM_LBUTTONDOWN, InputSystem::OnMouseLeftButtonDown);
		REGISTER_WINDOWS_EVENT_SUBSCRIBER(WM_LBUTTONUP,   InputSystem::OnMouseLeftButtonUp);
		REGISTER_WINDOWS_EVENT_SUBSCRIBER(WM_RBUTTONDOWN, InputSystem::OnMouseRightButtonDown);
		REGISTER_WINDOWS_EVENT_SUBSCRIBER(WM_RBUTTONUP,   InputSystem::OnMouseRightButtonUp);
		REGISTER_WINDOWS_EVENT_SUBSCRIBER(WM_MOUSEMOVE,   InputSystem::OnMouseMove);

		// Controller is handled differently - it is not event driven
	}

	InputSystem::~InputSystem()
	{
		UNREGISTER_SYSTEM_UPDATE_EVENT_SUBSCRIBER(InputSystem)

		// Keyboard 
		UNREGISTER_WINDOWS_EVENT_SUBSCRIBER(WM_KEYDOWN,     InputSystem::OnKeyboardKeyDown);
		UNREGISTER_WINDOWS_EVENT_SUBSCRIBER(WM_KEYUP,       InputSystem::OnKeyboardKeyUp);

		// Mouse
		UNREGISTER_WINDOWS_EVENT_SUBSCRIBER(WM_LBUTTONDOWN, InputSystem::OnMouseLeftButtonDown);
		UNREGISTER_WINDOWS_EVENT_SUBSCRIBER(WM_LBUTTONUP,   InputSystem::OnMouseLeftButtonUp);
		UNREGISTER_WINDOWS_EVENT_SUBSCRIBER(WM_RBUTTONDOWN, InputSystem::OnMouseRightButtonDown);
		UNREGISTER_WINDOWS_EVENT_SUBSCRIBER(WM_RBUTTONUP,   InputSystem::OnMouseRightButtonUp);
		UNREGISTER_WINDOWS_EVENT_SUBSCRIBER(WM_MOUSEMOVE,   InputSystem::OnMouseMove);
	}

	void InputSystem::Update(nabi::GameTime const& /*gameTime*/)
	{
		UpdateKeyboard();
		UpdateMouse();
		UpdateControllers();
	}

	void InputSystem::UpdateKeyboard() const
	{
		SComp::KeyboardState& state = InputModule::GetKeyboardState(m_Context);

		state.m_PreviousKeyStates = state.m_CurrentKeyStates;
		state.m_CurrentKeyStates = state.m_PendingKeyStates;
	}

	void InputSystem::UpdateMouse() const
	{
		SComp::MouseState& state = InputModule::GetMouseState(m_Context);

		state.m_PreviousButtonStates = state.m_CurrentButtonStates;
		state.m_CurrentButtonStates = state.m_PendingButtonStates;
	}

	void InputSystem::UpdateControllers() const
	{
		SComp::ControllerState& state = InputModule::GetControllerState(m_Context);
		state.m_PreviousControllerStates = state.m_CurrentControllerStates;

		for (DWORD i = 0; i < XUSER_MAX_COUNT; ++i)
		{
			// Grab the controllers state from xinput
			XINPUT_STATE xInputState;
			ZeroMemory(&xInputState, sizeof(XINPUT_STATE));

			// Cache the current controller
			SComp::ControllerState::Controller& controller = state.m_CurrentControllerStates[i];

			// If the controller is connected, populate the struct
			if (XInputGetState(i, &xInputState) == 0)
			{
				// Get the controller's XINPUT state 
				XINPUT_GAMEPAD& gamepad = xInputState.Gamepad;

				// Connected
				controller.m_Connected = true;

				// Buttons
				controller.m_Buttons = gamepad.wButtons;

				// Sticks
				float constexpr stickConversion = SComp::ControllerState::c_StickConversion;
				controller.m_Axis[0] = static_cast<float>(gamepad.sThumbLX) / stickConversion;
				controller.m_Axis[1] = static_cast<float>(gamepad.sThumbLY) / stickConversion;
				controller.m_Axis[2] = static_cast<float>(gamepad.sThumbRX) / stickConversion;
				controller.m_Axis[3] = static_cast<float>(gamepad.sThumbRY) / stickConversion;

				// Triggers
				float constexpr triggerConversion = SComp::ControllerState::c_TriggerConversion;
				controller.m_Axis[4] = static_cast<float>(gamepad.bLeftTrigger)  / triggerConversion;
				controller.m_Axis[5] = static_cast<float>(gamepad.bRightTrigger) / triggerConversion;
			}
			else
			{
				// Null out the Controller structure if its not connected
				controller.m_Connected = false;
				controller.m_Buttons = 0x0000;
				controller.m_Axis.fill(0.0F);
			}
		}
	}

	void InputSystem::OnKeyboardKeyDown(WPARAM const wParam, LPARAM const /*lParam*/) const
	{
		InputModule::GetKeyboardState(m_Context).m_PendingKeyStates[wParam] = true;
	}

	void InputSystem::OnKeyboardKeyUp(WPARAM const wParam, LPARAM const /*lParam*/) const
	{
		InputModule::GetKeyboardState(m_Context).m_PendingKeyStates[wParam] = false;
	}

	void InputSystem::OnMouseLeftButtonDown(WPARAM const /*wParam*/, LPARAM const /*lParam*/) const
	{
		InputModule::GetMouseState(m_Context).m_PendingButtonStates[GetInputCode(InputCode::Mouse_LeftButton)] = true;
	}

	void InputSystem::OnMouseLeftButtonUp(WPARAM const /*wParam*/, LPARAM const /*lParam*/) const
	{
		InputModule::GetMouseState(m_Context).m_PendingButtonStates[GetInputCode(InputCode::Mouse_LeftButton)] = false;
	}

	void InputSystem::OnMouseRightButtonDown(WPARAM const /*wParam*/, LPARAM const /*lParam*/) const
	{
		InputModule::GetMouseState(m_Context).m_PendingButtonStates[GetInputCode(InputCode::Mouse_RightButton)] = true;
	}

	void InputSystem::OnMouseRightButtonUp(WPARAM const /*wParam*/, LPARAM const /*lParam*/) const
	{
		InputModule::GetMouseState(m_Context).m_PendingButtonStates[GetInputCode(InputCode::Mouse_RightButton)] = false;
	}

	void InputSystem::OnMouseMove(WPARAM const /*wParam*/, LPARAM const lParam) const
	{
		POINTS const mousePosition = MAKEPOINTS(lParam);
		InputModule::GetMouseState(m_Context).m_MousePosition = dx::XMFLOAT2(
			static_cast<float>(mousePosition.x), 
			static_cast<float>(mousePosition.y)
		);
	}
} // namespace ecs
