#include "Core.h"

#include "MockObjects/MockCustomDataType.h"

#include "ReflectionIncludes.h"

#ifdef RUN_TESTS

namespace nabi::Tests
{
	REFLECT_DATA_TYPE_DEFAULT(MockCustomDataType)
} // namespace nabi::Tests::MockCustomDataType

#endif // #ifdef RUN_TESTS
