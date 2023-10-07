#pragma once
#include "EngineCore.h"

#include "entt.h"

#include "Context.h"

namespace nabi::ECS
{
	/// <summary>
	/// Base class for Systems. The registry and ids are things all systems will need. But no virtual inheritance!
	/// Could also be useful if I ever need to group all systems
	/// </summary>
	class SystemBase abstract
	{
	public:
		SystemBase(nabi::Context& context, entt::hashed_string const systemId, entt::hashed_string const systemGroupId) NABI_NOEXCEPT
			: m_Context(context)
			, c_SystemId(systemId)
			, c_SystemGroupId(systemGroupId)
		{
		}

		[[nodiscard]] inline entt::hashed_string const& GetSystemId() const NABI_NOEXCEPT { return c_SystemId; }
		[[nodiscard]] inline entt::hashed_string const& GetSystemGroupId() const NABI_NOEXCEPT { return c_SystemGroupId; }

	protected:
		nabi::Context& m_Context;

	private:
		entt::hashed_string const c_SystemId;
		entt::hashed_string const c_SystemGroupId;
	};
} // namespace nabi::ECS
