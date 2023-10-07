#include "Core.h"

#include "MockObjects/MockEnum.h"

#include "ReflectionIncludes.h"

#ifdef RUN_TESTS

namespace nabi::Tests
{
	REFLECT_ENUM_BEGIN_DEFAULT(MockEnum)
		REFLECT_ENUM_VALUE(MockEnum::None, "None")
		REFLECT_ENUM_VALUE_DEFAULT(MockEnum::Ben)
		REFLECT_ENUM_VALUE_DEFAULT(MockEnum::Kyra)
		REFLECT_ENUM_VALUE_DEFAULT(MockEnum::AnotherEntry)
		REFLECT_ENUM_VALUE_DEFAULT(MockEnum::ENUM_COUNT)
	REFLECT_ENUM_END(MockEnum)

	REFLECT_ENUM_BEGIN_DEFAULT(MockEnumFlags)
		REFLECT_ENUM_VALUE_DEFAULT(MockEnumFlags::One)
		REFLECT_ENUM_VALUE_DEFAULT(MockEnumFlags::Two)
		REFLECT_ENUM_VALUE_DEFAULT(MockEnumFlags::Three)
		REFLECT_ENUM_VALUE_DEFAULT(MockEnumFlags::All)
	REFLECT_ENUM_END(MockEnumFlags)

	REFLECT_ENUM_UNDERLYING_BEGIN_DEFAULT(MockEnumFlagsUnderlying)
		REFLECT_ENUM_VALUE_DEFAULT(MockEnumFlagsUnderlying::One)
		REFLECT_ENUM_VALUE_DEFAULT(MockEnumFlagsUnderlying::Two)
		REFLECT_ENUM_VALUE_DEFAULT(MockEnumFlagsUnderlying::Three)
		REFLECT_ENUM_VALUE_DEFAULT(MockEnumFlagsUnderlying::All)
	REFLECT_ENUM_END(MockEnumFlagsUnderlying)

	namespace NamespacedMockEnum
	{
		REFLECT_ENUM_BEGIN_DEFAULT(Enum)
			REFLECT_ENUM_VALUE_DEFAULT(Enum::TestOne)
			REFLECT_ENUM_VALUE_DEFAULT(Enum::TestOne)
		REFLECT_ENUM_END(Enum)
	} // namespace NamespacedMockEnum
} // namespace nabi::Tests

#endif // ifdef RUN_TESTS
