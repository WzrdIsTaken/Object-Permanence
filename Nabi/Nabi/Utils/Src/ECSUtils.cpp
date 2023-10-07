#include "EngineCore.h"

#include "ECSUtils.h"

#include "DebugUtils.h"

namespace nabi::ECSUtils
{
	std::string GetEntityUUIDAsString(entt::entity const entity) NABI_NOEXCEPT
	{
		// Entt is already tracking a UUID for each entity, so we might as well use it
		// The id for each entity is guaranteed to be unique, and will be recycled if the entity is destroyed

		// Just to avoid any unexpected surprises...
		STATIC_ASSERT(std::is_same<ENTT_ID_TYPE, uint32_t>::value, "Was expecting ENTT_ID_TYPE to be a uint32!");

		ENTT_ID_TYPE const entityId = static_cast<ENTT_ID_TYPE>(entity);
		return std::to_string(entityId);
	}
} // namespace nabi::ECSUtils
