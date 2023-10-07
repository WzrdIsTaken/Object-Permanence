#pragma once
#include "Core.h"

#include "Context.h"

namespace ecs::ReflectionModule
{
	struct Constraints
	{
		// Debug only checks
		bool m_ExpectStatic; // (or free)
		bool m_ExpectConst;

		// Release + debug flow
		bool m_CanNotExist;
	};

	Constraints constexpr c_ConstraintsDefaultSettings
	{
		// Note that m_CanNotExist is true. Components which have the option to have reflected function data members
		// might not always have them filled. In this case, we don't want to assert as this is the expected behaviour.
		// Eg - The ColliderComponent/CollisionSystem
		// If constraints is not null, then this is the expected default

		.m_ExpectStatic = false,
		.m_ExpectConst = false,
		.m_CanNotExist = true
	};

	Constraints constexpr c_EventConstraints
	{
		// Note that m_ExpectStatic is true. The functions for events, eg the collision's OnCollisionEnter/Exit events have to be 
		// static or free functions. This is because when the event is fired, we don't know the (eg system) instance the function to 
		// be called is on.

		.m_ExpectStatic = true,
		.m_ExpectConst = false,
		.m_CanNotExist = true
	};

	template<typename... Args>
	entt::meta_any CallReflectedFunction(nabi::Context const& /*context*/, 
		entt::hashed_string const type, entt::hashed_string const function, Constraints const* const constraints, Args&&... args)
	{
		// Can StringConverter::ConvertFromString use this? The reason its not currently using this is because this logic was written a while after StringConverter.
		// But... its also kinda good that the ecs/StringConverter are seperated. Doesn't make a big difference either way I guess.

		entt::meta_any result = {};

		if (type != nabi::Reflection::ReflectionGlobals::c_InvalidType && function != nabi::Reflection::ReflectionGlobals::c_InvalidFunction)
		{
			entt::meta_type const reflectedType = entt::resolve(type);
			entt::meta_func const reflectedFunc = reflectedType.func(function);

			if (reflectedFunc)
			{
				// Debug flow
				ASSERT_CODE
				(
					bool functionValid = true;

					if (constraints)
					{
						if (constraints->m_ExpectStatic && !reflectedFunc.is_static())
						{
							functionValid = false;
						}
						if (constraints->m_ExpectConst && !reflectedFunc.is_const())
						{
							functionValid = false;
						}
					}

					if (functionValid)
					{
						result = reflectedFunc.invoke(reflectedType, std::forward<Args>(args)...);
					}
					else
					{
						ASSERT_FAIL("The reflected function didn't meet the constraints");
					}
				)

				// Release flow
				FINAL_CODE
				(
					result = reflectedFunc.invoke(reflectedType, std::forward<Args>(args)...);
				)
			}
			else
			{
				if (constraints && !constraints->m_CanNotExist)
				{
					// Currently, this m_CanNotExist check is only used to prevent a debug only assert. However, the same expectation is in final 

					ASSERT_FAIL("The type " << type.data() << " does not have a " << function.data() << " method");
				}
			}
		}
		else
		{
			if (constraints && !constraints->m_CanNotExist)
			{
				ASSERT_FAIL("The reflected type or function has not been set");
			}
		}

		return result;
	}
} // namespace ecs::ReflectionModule
