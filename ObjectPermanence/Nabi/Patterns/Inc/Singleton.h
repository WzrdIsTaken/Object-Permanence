#pragma once
#include "EngineCore.h"

#include <memory>
#include <string>

namespace nabi::Patterns
{
#define TSINGLETON_INSTANCE(_class) std::unique_ptr<_class> nabi::Patterns::TSingleton<_class>::s_Instance = nullptr;

	namespace SingletonHelpers
	{
		/// <summary>
		/// This is a little jank and if I think of a better solution I will do it. Whats happening is that DebugUtils includes Singleton, 
		/// and so if the Singleton header includes DebugUtils then we have circular dependency.
		/// I can't declare Singleton's functions in the cpp because its a template class. 
		/// So, my solution is to have this free function which calls ASSERT with its contents in the cpp, and Singleton can call this instead.
		/// This should be the only time I need to do something like this. If I think of a better solution I'll replace it!
		/// </summary>
		/// <param name="assertCondition">- The condition that will trigger the assertion</param>
		/// <param name="assertMessge">- The message displayed in the assertion</param>
		void CallAssert(bool const assertCondition, std::string_view const assertMessge) NABI_NOEXCEPT;
	} // namespace nabi::Patterns::SingletonHelpers

	template<class T>
	class TSingleton
	{
	public:
		static void CreateInstance() NABI_NOEXCEPT
		{
			SingletonHelpers::CallAssert(s_Instance == nullptr, "Your creating a Singleton instance when one already exists!");
			s_Instance = std::make_unique<T>();
		}

		template<typename... Args>
		static void CreateInstance(Args... args) NABI_NOEXCEPT
		{
			SingletonHelpers::CallAssert(s_Instance == nullptr, "Your creating a Singleton instance when one already exists!");
			s_Instance = std::make_unique<T>(std::forward<Args>(args...));
		}

		[[nodiscard]] static bool IsInstanceValid() NABI_NOEXCEPT
		{
			return s_Instance != nullptr;
		}

		[[nodiscard]] static T* Instance() NABI_NOEXCEPT
		{
			SingletonHelpers::CallAssert(s_Instance != nullptr, "Trying to access the singleton instance when it hasn't been created yet!");
			return s_Instance.get();
		}

	private:
		static std::unique_ptr<T> s_Instance;
	};
} // namespace nabi::Patterns

