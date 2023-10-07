#pragma once
#include "Demo\DemoCore.h"

#ifdef INCLUDE_DEMO

#include "ComponentBase.h"
#include "InputCodes.h"

namespace ecs::SComp
{
	struct DemoPropertiesComponent final : nabi::ECS::ComponentBase
	{
		nabi::Input::InputCode m_LoadAsteroidGroupKey;
		nabi::Input::InputCode m_UnloadAsteroidGroupKey;

		nabi::Input::InputCode m_PlaySoundEffectKey;

		DemoPropertiesComponent()
			: m_LoadAsteroidGroupKey(nabi::Input::InputCode::Key_O)
			, m_UnloadAsteroidGroupKey(nabi::Input::InputCode::Key_P)
			, m_PlaySoundEffectKey(nabi::Input::InputCode::Key_L)
		{
		}
	};
} // namespace ecs::SComp

#endif // ifdef INCLUDE_DEMO
