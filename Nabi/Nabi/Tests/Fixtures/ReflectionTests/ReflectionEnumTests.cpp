#include "TestCore.h"

#include "MockObjects\MockEnum.h"
#include "ReflectionIncludes.h"

#ifdef RUN_TESTS

namespace nabi::Tests::ReflectionTests
{
	// Enum -> String functionality is currently not needed, I will implement if it becomes needed!

	// Check enums work as expected
	TEST(ReflectionTests, ParseEnum)
	{
		//Comparison<std::string> nameComparison("Ben");
		Comparison<MockEnum> valueComparison(MockEnum::Kyra);

		//nameComparison.m_Actual = nabi::Reflection::EnumConverter::EnumToString(20);
		valueComparison.m_Actual = nabi::Reflection::EnumConverter::StringToEnum<MockEnum>("Kyra"); // Wooyoung <3

		//COMPAIR_EQ(nameComparison);
		COMPAIR_EQ(valueComparison);
	}

	// Check enum flags work as expected
	TEST(ReflectionTests, ParseEnumFlags)
	{
		MockEnumFlags const mockFlags = nabi::Reflection::EnumConverter::StringToEnum<MockEnumFlags>("One|Two");
		MockEnumFlags const mockAll   = nabi::Reflection::EnumConverter::StringToEnum<MockEnumFlags>("All"    );

		EXPECT_TRUE (static_cast<bool> (mockFlags & MockEnumFlags::One  ));
		EXPECT_TRUE (static_cast<bool> (mockAll   & MockEnumFlags::Two  ));
		EXPECT_FALSE(static_cast<bool> (mockFlags & MockEnumFlags::Three));
	}

	// Check enum underlying reflection works
	TEST(ReflectionTests, ParseEnumUnderlyingType)
	{
		// This is useful when you want to make a generic system, which eg doesn't know the enums it will use because they are user defined,
		// so you can just use the enum's underlying type. Eg - with bitfields. Perhaps that made sense. Its kinda late rn...
		// Basically just see the collision system. This is how I envision it working rn

		Comparison<MockEnumFlagsUnderlyingType> valueComparison(1 << 1);
		Comparison<MockEnumFlagsUnderlyingType> flagsComparison(10ull);

		valueComparison.m_Actual = nabi::Reflection::EnumConverter::StringToEnumUnderlyingValue<MockEnumFlagsUnderlying>("One");
		flagsComparison.m_Actual = nabi::Reflection::EnumConverter::StringToEnumUnderlyingValue<MockEnumFlagsUnderlying>("One|Three");

		COMPAIR_EQ(valueComparison);
		COMPAIR_EQ(flagsComparison);
	}
} // namespace nabi::Tests::ReflectionTests

#endif // #ifdef RUN_TESTS
