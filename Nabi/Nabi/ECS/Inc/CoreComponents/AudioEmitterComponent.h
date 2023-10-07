#pragma once
#include "Core.h"

#include "AudioIncludes.h"

#include "ComponentBase.h"

namespace ecs
{
	struct AudioEmitterComponent final : public nabi::ECS::ComponentBase
	{
		typedef std::vector<FLOAT32> DSPMatrix;

		X3DAUDIO_EMITTER m_Emitter;
		X3DAUDIO_DSP_SETTINGS m_DSPSettings;
		DSPMatrix m_DSPMatrix;

		AudioEmitterComponent()
			: m_Emitter{}
			, m_DSPSettings{}
			, m_DSPMatrix{}
		{
		}
	};
} // namespace ecs
