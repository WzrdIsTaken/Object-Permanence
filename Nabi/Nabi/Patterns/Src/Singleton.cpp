#include "EngineCore.h"

#include "Singleton.h"

#include "DebugUtils.h"

namespace nabi::Patterns
{
	namespace SingletonHelpers
	{
		void CallAssert(bool const assertCondition, std::string_view const assertMessge) NABI_NOEXCEPT
		{
			ASSERT(assertCondition, assertMessge);
		}
	} // namespace nabi::Patterns::SingletonHelpers
} // namespace nabi::Patterns
