#pragma once
#include "Core.h"

#include "ComponentBase.h"

namespace ecs::TComp
{
	// Used in conjunction with DrawPerspectiveTagComponent

	struct DrawOrthographicTagComponent final : public nabi::ECS::ComponentBase
	{
	};
} // namespace ecs::TComp
