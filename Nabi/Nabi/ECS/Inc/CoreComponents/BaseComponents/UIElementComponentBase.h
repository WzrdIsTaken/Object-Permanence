#pragma once
#include "Core.h"

#include "entt.h"

#include "ComponentBase.h"
#include "InputCodes.h"
#include "ReflectionGlobals.h"

namespace ecs::BComp
{
	// NOTE - If more ui components are added, UISystem needs to be updated(see the 'UIElementComponentBase& uiElement = m_Context.m_Registry.get<ButtonComponent>(entity);' line)

	// This is because if more ui components are added, it would get tedious to reflect all the base members again (eg, in the future might have a slider component)
#define REFLECT_UI_COMPONENT_BASE_PROPERTIES(uiComponentType) \
	REFLECT_COMPONENT_PROPERTY(CONCAT(uiComponentType, ::m_Scene), "Scene") \
	REFLECT_COMPONENT_PROPERTY(CONCAT(uiComponentType, ::m_Action), "Action") \
	REFLECT_COMPONENT_PROPERTY(CONCAT(uiComponentType, ::m_InputType), "InputType") \
	REFLECT_COMPONENT_PROPERTY(CONCAT(uiComponentType, ::m_ActivationKey), "ActivationKey") \
	REFLECT_COMPONENT_PROPERTY(CONCAT(uiComponentType, ::m_AltInputType), "AltInputType") \
	REFLECT_COMPONENT_PROPERTY(CONCAT(uiComponentType, ::m_AltActivationKey), "AltActivationKey") \
	REFLECT_COMPONENT_PROPERTY(CONCAT(uiComponentType, ::m_Controller), "Controller") \
	REFLECT_COMPONENT_PROPERTY(CONCAT(uiComponentType, ::m_Interactable), "Interactable") \
	REFLECT_COMPONENT_PROPERTY(CONCAT(uiComponentType, ::m_Active), "Active") \
	REFLECT_COMPONENT_PROPERTY(CONCAT(uiComponentType, ::m_Selected), "StartSelected")

#pragma warning( push )
#pragma warning( disable : 26495 ) // [var] is unitialized

	struct UIElementComponentBase abstract : public nabi::ECS::ComponentBase
	{
		entt::hashed_string m_Scene;
		entt::hashed_string m_Action;

		nabi::Input::InputType m_InputType;
		nabi::Input::InputCode m_ActivationKey;

		nabi::Input::InputType m_AltInputType;
		nabi::Input::InputCode m_AltActivationKey;

		nabi::Input::Controller m_Controller; // Only set if you're, eg, making a 2 player game. Defaults to master

		bool m_Interactable;
		bool m_Active; // can be used for eg, setting if a button is hovered
		bool m_Selected;

		UIElementComponentBase()
			: m_Scene(nabi::Reflection::ReflectionGlobals::c_InvalidType)
			, m_Action(nabi::Reflection::ReflectionGlobals::c_InvalidFunction)
			, m_InputType(nabi::Input::InputType::Invalid)
			, m_ActivationKey(nabi::Input::InputCode::Invalid)
			, m_AltInputType(nabi::Input::InputType::Invalid)
			, m_AltActivationKey(nabi::Input::InputCode::Invalid)
			, m_Controller(nabi::Input::Controller::MasterController)
			, m_Interactable(true)
			, m_Active(true) // defaulted to true because if their is no special button behavior we still want to detect inputs from it
			, m_Selected(false)
		{
		}
	};

#pragma warning( pop )
} // namespace ecs::BComp
