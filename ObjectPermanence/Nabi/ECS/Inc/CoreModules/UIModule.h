#pragma once
#include "Core.h"

#include <any>

#include "Context.h"
#include "CoreSingletonComponents\UIStateComponent.h"

namespace ecs
{
	struct CameraComponent;
} // namespace ecs

namespace ecs::UIModule
{
	// --- UI State ---

	[[nodiscard]] inline SComp::UIStateComponent const& GetUIStateComponent(nabi::Context const& context)
	{
		return context.m_Registry.get<SComp::UIStateComponent>(context.m_SingletonEntites.at(nabi::Context::SingletonEntities::Input));
	}
	[[nodiscard]] inline SComp::UIStateComponent& GetUIStateComponent(nabi::Context& context)
	{
		return const_cast<SComp::UIStateComponent&>(GetUIStateComponent(const_cast<nabi::Context const&>(context)));
	}

	[[nodiscard]] inline SComp::UIStateComponent const* const TryGetUIStateComponent(nabi::Context const& context)
	{
		return context.m_Registry.try_get<SComp::UIStateComponent>(context.m_SingletonEntites.at(nabi::Context::SingletonEntities::Input));
	}
	[[nodiscard]] inline SComp::UIStateComponent* const TryGetUIStateComponent(nabi::Context& context)
	{
		return const_cast<SComp::UIStateComponent* const>(TryGetUIStateComponent(const_cast<nabi::Context const&>(context)));
	}

	enum class GetMode : int
	{
		First,
		FirstEnabled,

		ENUM_COUNT
	};
	[[nodiscard]] UISceneComponent::UISceneEntities const* const GetCurrentUISceneEntities(nabi::Context const& context, GetMode const getMode);
	[[nodiscard]] inline UISceneComponent::UISceneEntities* const GetCurrentUISceneEntities(nabi::Context& context, GetMode const getMode)
	{
		return const_cast<UISceneComponent::UISceneEntities* const>(GetCurrentUISceneEntities(const_cast<nabi::Context const&>(context), getMode));
	}

	// --- UI Storage ---

	[[nodiscard]] inline SComp::UIStorageComponent const& GetUIStorageComponent(nabi::Context const& context)
	{
		return context.m_Registry.get<SComp::UIStorageComponent>(context.m_SingletonEntites.at(nabi::Context::SingletonEntities::Input));
	}
	[[nodiscard]] inline SComp::UIStorageComponent& GetUIStorageComponent(nabi::Context& context)
	{
		return const_cast<SComp::UIStorageComponent&>(GetUIStorageComponent(const_cast<nabi::Context const&>(context)));
	}

	template<typename T>
	[[nodiscard]] T GetStickyAs(nabi::Context const& context, SComp::UIStorageComponent::Sticky const sticky)
	{
		std::any const sticky = GetSticky(context, sticky);

		// Debug flow
		ASSERT_CODE
		(
			try
			{
				T const stickyAsT = std::any_cast<T>(sticky);
				return stickyAsT;
			}
			catch (std::bad_any_cast const& exception)
			{
				ASSERT_FAIL("Couldn't get sticky " << sticky << " as type T!");
				return {};
			}
		)

		// Release flow
		FINAL_CODE
		(
			T const stickyAsT = std::any_cast<T>(sticky);
			return stickyAsT;
		)
	};
	[[nodiscard]] inline std::any GetSticky(nabi::Context const& context, SComp::UIStorageComponent::Sticky const sticky)
	{ 
		SComp::UIStorageComponent const& uiStorage = GetUIStorageComponent(context);
		return uiStorage.m_Storage.at(sticky);
	};

	[[nodiscard]] inline bool HasSticky(SComp::UIStorageComponent const& uiStorage, SComp::UIStorageComponent::Sticky const sticky)
	{
		return uiStorage.m_Storage.find(sticky) != uiStorage.m_Storage.end();
	}
	[[nodiscard]] inline bool HasSticky(nabi::Context const& context, SComp::UIStorageComponent::Sticky const sticky)
	{ 
		SComp::UIStorageComponent const& uiStorage = GetUIStorageComponent(context);
		return HasSticky(uiStorage, sticky);
	};

	inline void SetSticky(nabi::Context& context, SComp::UIStorageComponent::Sticky const sticky, std::any const value)
	{
		SComp::UIStorageComponent& uiStorage = GetUIStorageComponent(context);
		if (HasSticky(uiStorage, sticky))
		{
			LOG(LOG_PREP, LOG_WARN, LOG_CATEGORY_UI, "Overriding an existing sticky's  (" << sticky << ") value!", LOG_END);
			uiStorage.m_Storage.at(sticky) = value;
		}
		else
		{
			std::pair<SComp::UIStorageComponent::Sticky, std::any> const pair = { sticky, value };
			uiStorage.m_Storage.emplace(pair);
		}
	}

	inline void ClearSticky(nabi::Context& context, SComp::UIStorageComponent::Sticky const sticky)
	{
		CONDITIONAL_LOG(!HasSticky(context, sticky), LOG_PREP, LOG_INFO, LOG_CATEGORY_UI, "Clearing a sticky (" << sticky << ") which doesn't exist!", LOG_END);

		SComp::UIStorageComponent& uiStorage = GetUIStorageComponent(context);
		uiStorage.m_Storage.erase(sticky);
	};
	inline void ClearStickies(nabi::Context& context)
	{
		LOG(LOG_PREP, LOG_INFO, LOG_CATEGORY_UI, "Clearing all stickies from UIStorage!", LOG_END);

		SComp::UIStorageComponent& uiStorage = GetUIStorageComponent(context);
		uiStorage.m_Storage.clear();
	};

	// --- UI Input ---

	[[nodiscard]] bool CheckIfMouseIsOverElement(nabi::Context const& context, 
		CameraComponent const& perspectiveCamera, entt::entity const elementEntity);
	[[nodiscard]] bool CheckIfMouseIsOverElement(nabi::Context const& context, 
		dx::XMFLOAT2 const elementPosition, dx::XMFLOAT2 const elementDimensions);
	[[nodiscard]] bool CheckIfMouseIsOverElement(nabi::Context const& context, 
		dx::XMFLOAT2 const mousePosition, dx::XMFLOAT2 const elementPosition, dx::XMFLOAT2 const elementDimensions);
} // namespace ecs::UIModule

