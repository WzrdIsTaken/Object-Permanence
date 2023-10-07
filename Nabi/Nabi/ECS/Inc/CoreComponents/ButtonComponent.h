#pragma once
#include "Core.h"

#include "CoreComponents\BaseComponents\UIElementComponentBase.h"

namespace ecs
{
	struct ButtonComponent final : public BComp::UIElementComponentBase
	{
		using UIElementComponentBase::UIElementComponentBase;
	};
} // namespace ecs
