#pragma once
#include "Core.h"

#ifdef RUN_TESTS

namespace nabi::Tests
{
	enum class MockEnum 
	{
		None,
		Ben  = 20,
		Kyra = 27,
		AnotherEntry,
		ENUM_COUNT
	};

	enum class MockEnumFlags
	{
		One   = 1 << 1,
		Two   = 1 << 2,
		Three = 1 << 3,

		All   = ~0
	};
	DEFINE_ENUM_FLAG_OPERATORS(MockEnumFlags)

	typedef std::uint64_t MockEnumFlagsUnderlyingType;
	enum class MockEnumFlagsUnderlying : MockEnumFlagsUnderlyingType
	{
		One   = 1 << 1,
		Two   = 1 << 2,
		Three = 1 << 3,

		All   = ~0
	};
	DEFINE_ENUM_FLAG_OPERATORS(MockEnumFlagsUnderlying)

	namespace NamespacedMockEnum
	{
		enum Enum : int
		{
			TestOne,
			TestTwo
		};
	} // namespace NamespacedMockEnum
} // namespace nabi::Tests

#endif // ifdef RUN_TESTS
