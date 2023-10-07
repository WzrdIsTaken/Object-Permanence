#include "Core.h"

#include "CoreComponents\ButtonComponent.h"

#include "ReflectionIncludes.h"

namespace ecs
{
	REFLECT_COMPONENT_BEGIN_DEFAULT(ButtonComponent)
		REFLECT_UI_COMPONENT_BASE_PROPERTIES(ButtonComponent)
	REFLECT_COMPONENT_END(ButtonComponent)
} // namespace ecs
