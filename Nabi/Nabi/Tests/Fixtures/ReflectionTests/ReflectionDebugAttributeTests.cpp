#include "TestCore.h"

#include "pugixml.hpp"

#include "BuildUtils.h"
#include "TestUtils.h"
#include "XmlParser.h"

// This is kinda jank test ngl, I gotta get better at writing these things...

#ifdef RUN_TESTS

namespace nabi::Tests::ReflectionTests
{
#ifdef USE_DEBUG_UTILS
	// Checks that debug attribute works as expected
	TEST(ReflectionTests, ParseEntityWithDebugAttribute)
	{
		BUILD_CONFIGURATION_CHANGE_BEGIN

		using namespace nabi::BuildUtils;

		// Mock objects
		entt::registry registry;
		std::string const docPath = "Nabi/Tests/Data/Reflection/test_debug_entity_file.xml";
		nabi::Reflection::XmlParser xmlParser{};

		// Release mode test
		c_BuildConfiguration = BuildConfiguration::Release;

		pugi::xml_document doc = xmlParser.LoadDocument(docPath);
		xmlParser.ParseEntities(doc, registry);

		Comparison<size_t> entityCount(0, registry.size());
		COMPAIR_EQ(entityCount);

		registry.clear();

		// Debug mode test
		c_BuildConfiguration = BuildConfiguration::Debug;

		doc = xmlParser.LoadDocument(docPath);
		xmlParser.ParseEntities(doc, registry);

		entityCount = Comparison<size_t>(1, registry.size());
		COMPAIR_EQ(entityCount);

		registry.clear();

		BUILD_CONFIGURATION_CHANGE_END
	}
#endif // ifdef USE_DEBUG_UTILS
} // namespace nabi::Tests::ReflectionTests

#endif // #ifdef RUN_TESTS
