#include "EngineCore.h"

#include "EntityPropertyList.h"

#include "DebugUtils.h"

namespace nabi::ECS
{
	void EntityPropertyList::AddProperty(std::string_view const component, std::string_view const propertyName, std::string_view const propertyValue) NABI_NOEXCEPT
	{
		Reflection::PropertyData propertyOverride;
		propertyOverride.m_Id = entt::hashed_string(propertyName.data());
		propertyOverride.m_Value = entt::hashed_string(propertyValue.data());

		PropertyComponentPair propertyComponentPair;
		propertyComponentPair.m_ComponentHash = entt::hashed_string(component.data());
		propertyComponentPair.m_PropertyOverride = std::move(propertyOverride);

		m_OverriddenProperties.emplace_back(propertyComponentPair);
	}

	void EntityPropertyList::UpdateProperty(std::string_view const component, std::string_view const propertyName, std::string_view const propertyValue) NABI_NOEXCEPT
	{
		PropertyComponentPair& propertyComponentPair = FindPropertyComponentPair(component, propertyName);
		propertyComponentPair.m_PropertyOverride.m_Value = entt::hashed_string(propertyValue.data());
	}

	void EntityPropertyList::RemoveProperty(std::string_view const component, std::string_view const propertyName) NABI_NOEXCEPT
	{
		PropertyComponentPair& propertyComponentPair = FindPropertyComponentPair(component, propertyName);
		m_OverriddenProperties.erase(std::remove(m_OverriddenProperties.begin(), m_OverriddenProperties.end(), propertyComponentPair));
	}

	std::vector<EntityPropertyList::PropertyComponentPair> const& EntityPropertyList::GetOverridenProperties() const NABI_NOEXCEPT
	{
		return m_OverriddenProperties;
	}

	EntityPropertyList::PropertyComponentPair& EntityPropertyList::FindPropertyComponentPair(std::string_view const component, std::string_view const propertyName) NABI_NOEXCEPT
	{
		entt::hashed_string componentHash = entt::hashed_string(component.data());
		entt::hashed_string propertyHash  = entt::hashed_string(propertyName.data());

		auto result = std::find_if(m_OverriddenProperties.begin(), m_OverriddenProperties.end(), 
			[componentHash, propertyHash](PropertyComponentPair const& propertyComponentPair) -> bool
			{
				return propertyComponentPair.m_ComponentHash == componentHash &&
					propertyComponentPair.m_PropertyOverride.m_Id == propertyHash;
			});

		ASSERT_FATAL(result != m_OverriddenProperties.end(), "Trying to find a property override which doesn't exist!");
		return *result;
	}
} // namespace nabi::ECS
