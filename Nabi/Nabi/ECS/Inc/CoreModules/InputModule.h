#pragma once
#include "Core.h"

#include "Context.h"
#include "InputCodes.h"
#include "InputState.h"
#include "CoreSingletonComponents\InputStateComponent.h"

namespace ecs::InputModule
{
	// These functions query the InputStateComponent to check key/mouse/button state
	[[nodiscard]] nabi::Input::InputState GetKeyboardKey(nabi::Context const& context, nabi::Input::InputCode const keyCode);

	[[nodiscard]] nabi::Input::InputState GetMouseButton(nabi::Context const& context, nabi::Input::InputCode const buttonCode);
	[[nodiscard]] dx::XMFLOAT2 GetMousePosition(nabi::Context const& context);

	[[nodiscard]] nabi::Input::InputState GetControllerButton(nabi::Context const& context, 
		nabi::Input::Controller const controllerCode, nabi::Input::InputCode const buttonCode);
	[[nodiscard]] float GetControllerAxis(nabi::Context const& context, nabi::Input::Controller const controllerCode, nabi::Input::InputCode const axis);
	[[nodiscard]] bool GetControllerConnected(nabi::Context const& context, nabi::Input::Controller const controllerCode);

	// Input state getters
	[[nodiscard]] inline SComp::InputStateComponent const& GetInputStateComponent(nabi::Context const& context) {
		return context.m_Registry.get<SComp::InputStateComponent>(context.m_SingletonEntites.at(nabi::Context::SingletonEntities::Input));
	};
	[[nodiscard]] inline SComp::KeyboardState const& GetKeyboardState(nabi::Context const& context)     { return GetInputStateComponent(context).m_KeyboardState;   }
	[[nodiscard]] inline SComp::MouseState const& GetMouseState(nabi::Context const& context)           { return GetInputStateComponent(context).m_MouseState;      }
	[[nodiscard]] inline SComp::ControllerState const& GetControllerState(nabi::Context const& context) { return GetInputStateComponent(context).m_ControllerState; }

	[[nodiscard]] inline SComp::KeyboardState& GetKeyboardState(nabi::Context& context)     { return const_cast<SComp::KeyboardState&>(GetInputStateComponent(context).m_KeyboardState);     }
	[[nodiscard]] inline SComp::MouseState& GetMouseState(nabi::Context& context)           { return const_cast<SComp::MouseState&>(GetInputStateComponent(context).m_MouseState);           }
	[[nodiscard]] inline SComp::ControllerState& GetControllerState(nabi::Context& context) { return const_cast<SComp::ControllerState&>(GetInputStateComponent(context).m_ControllerState); }
} // namespace ecs
