#include "Core.h"

#include "CoreComponents\AudioEmitterComponent.h"

#include "ReflectionIncludes.h"

namespace ecs
{
	// For this component, non of the members need reflecting. Just add it to a entity and the members will get inited by CoreSystems\AudioSystem

	REFLECT_COMPONENT_BEGIN_DEFAULT(AudioEmitterComponent)
	REFLECT_COMPONENT_END(AudioEmitterComponent)
} // namespace ecs
