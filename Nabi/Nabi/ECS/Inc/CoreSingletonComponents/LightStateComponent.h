#pragma once
#include "Core.h"

#include "ComponentBase.h"

namespace ecs::SComp
{
	struct LightStateComponent final : public nabi::ECS::ComponentBase
	{
		unsigned int m_LightCount;
#ifndef USE_EVENT_SYSTEM_UPDATE
		bool m_UpdateLights;
#endif // ifndef USE_EVENT_SYSTEM_UPDATE

		LightStateComponent()
			: m_LightCount(0u)
#ifndef USE_EVENT_SYSTEM_UPDATE
			, m_UpdateLights(true)
#endif // ifndef USE_EVENT_SYSTEM_UPDATE
		{
		}
	};
} // namespace ecs::SComp
