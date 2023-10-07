#pragma once
#include "EngineCore.h"

#include "entt.h"

namespace nabi::ECS
{
	/// <summary>
	/// A lightweight wrapper around a vector of entities
	/// </summary>
	class EntityGroup final
	{
	public:
		typedef std::vector<entt::entity> Group;

		inline bool AddEntity(entt::entity const entity) NABI_NOEXCEPT
		{
			bool const entityAlreadyInGroup = FindEntity(entity);
			bool const entityAdded = !entityAlreadyInGroup;

			if (!entityAlreadyInGroup)
			{
				m_Entities.push_back(entity);
			}
			
			return entityAdded;
		}
		inline bool RemoveEntity(entt::entity const entity) NABI_NOEXCEPT
		{
			auto it = std::remove(m_Entities.begin(), m_Entities.end(), entity);
			bool const found = it != m_Entities.end();

			if (found)
			{
				m_Entities.erase(it);
			}
		}
		inline bool FindEntity(entt::entity const entity) const NABI_NOEXCEPT
		{
			auto it = std::find(m_Entities.begin(), m_Entities.end(), entity);
			bool const found = it != m_Entities.cend();

			return found;
		}
		inline void ForeachEntity(std::function<bool(entt::entity)> const& action) NABI_NOEXCEPT
		{
			std::for_each(m_Entities.begin(), m_Entities.end(), action);
		}

		inline Group& GetGroup() NABI_NOEXCEPT
		{
			return m_Entities;
		}
		inline Group const& GetGroup() const NABI_NOEXCEPT
		{
			return m_Entities;
		}

	private:
		Group m_Entities;
	};
} // namespace nabi::ECS
