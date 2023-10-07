#pragma once
#include "Core.h"

#include "ReflectionIncludes.h"

#include "InputCodes.h"
#include "InputState.h"
#include "SystemBase.h"

namespace ecs::BComp
{
	struct UIElementComponentBase;
} // namespace ecs::BComp
namespace nabi
{
	struct Context;
	class GameTime;
} // namespace nabi

namespace ecs
{
	class UISystem final : public nabi::ECS::SystemBase
	{
	public:
		UISystem(nabi::Context& context, entt::hashed_string const systemId, entt::hashed_string const systemGroupId);
		~UISystem();

		void Update(nabi::GameTime const& gameTime);

	private:
		struct SelectedUIElement
		{
			entt::entity m_Entity;
			std::reference_wrapper<BComp::UIElementComponentBase> m_Element;
		};
		struct InputInfo
		{
			nabi::Input::InputType m_InputType;
			nabi::Input::InputCode m_InputCode;
			nabi::Input::Controller m_Controller;
		};

		[[nodiscard]] nabi::Input::InputState GetInput(InputInfo const inputInfo, InputInfo const altInputInfo) const;

		void OnUISceneCreated(entt::registry& registry, entt::entity entity);
		void OnUISceneDestroyed(entt::registry& registry, entt::entity entity);

		REFLECT_PRIVATES(UISystem)
	};
} // namespace ecs
