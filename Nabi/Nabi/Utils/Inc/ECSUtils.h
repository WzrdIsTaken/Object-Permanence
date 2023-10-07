#pragma once
#include "EngineCore.h"

#include <string>

#include "entt.h"

namespace nabi::ECSUtils
{
	[[nodiscard]] std::string GetEntityUUIDAsString(entt::entity const entity) NABI_NOEXCEPT;
} // namespace nabi::ECSUtils
