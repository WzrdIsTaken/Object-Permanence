#pragma once
#include "EngineCore.h"

#include "entt.h"

// A collection of variables used throughout the reflection system, so it makes sense to group them in one place

namespace nabi::Reflection::ReflectionGlobals
{
	entt::hashed_string constexpr c_FromStringFunctionName = entt::hashed_string("FromString");
	entt::hashed_string constexpr c_AssignFunctionName = entt::hashed_string("Assign");

	entt::hashed_string constexpr c_InvalidType = entt::hashed_string("InvalidType");
	entt::hashed_string constexpr c_InvalidFunction = entt::hashed_string("InvalidFunction");
} // namespace nabi::Reflection::ReflectionGlobals
