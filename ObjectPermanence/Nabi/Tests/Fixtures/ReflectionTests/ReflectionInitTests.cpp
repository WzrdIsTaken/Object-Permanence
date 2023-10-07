#include "TestCore.h"

#include "pugixml.hpp"

#include "Context.h"
#include "CoreComponents/EntityInfoComponent.h"
#include "DirectXUtils.h"
#include "MetaObjectLookup.h"
#include "MockObjects\MockComponent.h"
#include "MockObjects\MockCustomDataType.h"
#include "MockObjects\MockEnum.h"
#include "MockObjects\MockSystem.h"
#include "ReflectionIncludes.h"
#include "XmlParser.h"

#ifdef RUN_TESTS

namespace nabi::Tests::ReflectionTests
{
	// Check system files can be loaded correctly
	TEST(ReflectionTests, ParseSystemsFromRoute)
	{
		// Mock objects
		nabi::Context context;
		context.m_Registry = {};
		std::string const routeDocPath = "Nabi/Tests/Data/Reflection/test_route_file.xml";

		// Deserialize data files
		nabi::Reflection::MetaObjectLookup systemsLookup{};
		nabi::Reflection::XmlParser xmlParser{};

		xmlParser.ParseXml(routeDocPath, context, &systemsLookup);

		// Get the system
		MockSystem mockSystem = std::move(systemsLookup.GetObject<MockSystem>("MockSystem"));

		// Test that the system data is what we expect
		Comparison<int> systemDataComparison{};
		systemDataComparison.m_Expected = 5;
		systemDataComparison.m_Actual = mockSystem.GetMockSystemData();

		COMPAIR_EQ(systemDataComparison);
	}

	// Check entity files can be loaded correctly
	TEST(ReflectionTests, ParseEntitiesFromRoute)
	{
		// Mock objects
		nabi::Context context;
		context.m_Registry = {};

		std::string const routeDocPath = "Nabi/Tests/Data/Reflection/test_route_file.xml";
		entt::registry& registry = context.m_Registry;

		// Deserialize data files
		nabi::Reflection::XmlParser xmlParser{};
		xmlParser.ParseXml(routeDocPath, context, nullptr);

		// Check that this is only one entity in the registry
		Comparison<size_t> numberOfEntities(1, registry.alive());
		COMPAIR_EQ(numberOfEntities);

		// Iterate over the registery and check for MockComponent
		auto mockComponentView = registry.view<MockComponent>();

		Comparison<int> intType(5); // 5 is the default value, IntType is unassigned in the xml
		Comparison<float> floatType(50.8f);
		Comparison<std::string> stringType("somestring");
		Comparison<MockCustomDataType> customType({ 27.3, true });
		Comparison<MockEnum> enumType(MockEnum::AnotherEntry);

		for (auto [entity, mockComponent] : mockComponentView.each())
		{
			intType.m_Actual    = mockComponent.m_IntType;
			floatType.m_Actual  = mockComponent.m_FloatType;
			stringType.m_Actual = mockComponent.m_StringType;
			customType.m_Actual = mockComponent.m_CustomType;
			enumType.m_Actual   = mockComponent.m_Enum;
		}

		COMPAIR_EQ(intType);
		COMPAIR_EQ(floatType);
		COMPAIR_EQ(stringType);
		COMPAIR_EQ(customType);
		COMPAIR_EQ(enumType);

		// Iterate over the registery and check for EntityInfoComponent
		auto entityInfoComponentView = registry.view<ecs::EntityInfoComponent>();

		Comparison<entt::hashed_string> entityGroup("TestGroup"_hs);
		Comparison<entt::hashed_string> entityName("TestEntity"_hs);

		for (auto [entity, entityInfoComponent] : entityInfoComponentView.each())
		{
			entityGroup.m_Actual = entityInfoComponent.m_EntityGroup;
			entityName.m_Actual = entityInfoComponent.m_EntityName;
		}

		COMPAIR_EQ(entityGroup);
		COMPAIR_EQ(entityName);
	}

	TEST(ReflectionTests, ParseLibraryEntities)
	{
		// Mock objects
		nabi::Context context;
		context.m_Registry = {};

		std::string const docPath = "Nabi/Tests/Data/Reflection/test_library_entity_file.xml";
		entt::registry& registry = context.m_Registry;

		// Deserialize data files
		nabi::Reflection::XmlParser xmlParser{};
		pugi::xml_document const doc = xmlParser.LoadDocument(docPath);
		xmlParser.ParseEntities(doc, context.m_Registry);

		// Check that this is only one entity in the registry
		Comparison<size_t> numberOfEntities(1, registry.alive());
		COMPAIR_EQ(numberOfEntities);

		// Iterate over the registery and check for MockComponentWithDirectXTypes
		auto mockComponentView = registry.view<MockComponentWithDirectXTypes>();

		Comparison<dx::XMFLOAT2> float2({ 10.0f, 10.0f });
		Comparison<dx::XMFLOAT3> float3({ 450.0f, 31.0f, 4.0f });
		Comparison<dx::XMINT2> int2({ -7, 4 });
		Comparison<dx::XMINT3> int3({ 1, 1, 1 });

		for (auto [entity, mockComponent] : mockComponentView.each())
		{
			float2.m_Actual = mockComponent.m_Float2;
			float3.m_Actual = mockComponent.m_Float3;
			int2.m_Actual = mockComponent.m_Int2;
			int3.m_Actual = mockComponent.m_Int3;
		}

		// ;_;
		using namespace nabi::DirectXUtils;
		bool const float2Equal = Float2Equal(float2.m_Expected, float2.m_Actual);
		bool const float3Equal = Float3Equal(float3.m_Expected, float3.m_Actual);
		bool const int2Equal = Int2Equal(int2.m_Expected, int2.m_Actual);
		bool const int3Equal = Int3Equal(int3.m_Expected, int3.m_Actual);

		EXPECT_TRUE(float2Equal);
		EXPECT_TRUE(float3Equal);
		EXPECT_TRUE(int2Equal);
		EXPECT_TRUE(int3Equal);
	}

	TEST(ReflectionTests, ParseContainerEntities)
	{
		// Mock objects
		nabi::Context context;
		context.m_Registry = {};

		std::string const docPath = "Nabi/Tests/Data/Reflection/test_container_entity_file.xml";
		entt::registry& registry = context.m_Registry;

		// Deserialize data files
		nabi::Reflection::XmlParser xmlParser{};
		pugi::xml_document const doc = xmlParser.LoadDocument(docPath);
		xmlParser.ParseEntities(doc, context.m_Registry);

		// Check that this is only one entity in the registry
		Comparison<size_t> numberOfEntities(1, registry.alive());
		COMPAIR_EQ(numberOfEntities);

		// Iterate over the registery and check for MockComponentWithContainers
		auto mockComponentView = registry.view<MockComponentWithContainers>();

		Comparison<std::vector<int>> intVector({ 2, 4, 8 });
		Comparison<std::vector<std::string>> stringVector({ "Hello!" });
		Comparison<std::map<int, int>> intMap({ std::pair(0, 9), std::pair(8, 7) });

		for (auto [entity, mockComponent] : mockComponentView.each())
		{
			intVector.m_Actual = mockComponent.m_IntVector.Get();
			stringVector.m_Actual = mockComponent.m_StringVector.Get();
			intMap.m_Actual = mockComponent.m_IntMap.Get();
		}

		// Test!
		COMPAIR_EQ(intVector);
		COMPAIR_EQ(stringVector);
		COMPAIR_EQ(intMap);
	}
} // namespace nabi::Tests::ReflectionTests

#endif // #ifdef RUN_TESTS

/*
	Update 15/06/23 - Container reflection was hacked in with CREATE_CONTAINER_WRAPPER. See AudioResourceComponent for an example.
	Update 16/06/23 - I've just written a test for reflected containers, see above!

	Container support is currently not implemented in the reflection system.
	I was doing this in the past, then realised I didn't actually need it.
	The way I was doing it was with a jank macro solution. This is not the play! Entt supports container reflection, we just have to use it.
	https://github.com/skypjack/entt/wiki/Crash-Course:-runtime-reflection-system#container-support
	I didn't want to use this originally because I think it would require rewriting some of the reflection system. However, the more I did
	the macro approach the worse it got - so I if this is ever needed I think we'll just have to bite the bullet.

	TEST(ReflectionTests, ParseContainerEntities)
	{
		// Mock objects
		nabi::Context context;
		context.m_Registry = {};

		std::string const docPath = "Nabi/Tests/Data/Reflection/test_container_entity_file.xml";
		entt::registry& registry = context.m_Registry;

		// Deserialize data files
		nabi::Reflection::XmlParser xmlParser{};
		pugi::xml_document const doc = xmlParser.LoadDocument(docPath);
		xmlParser.ParseEntities(doc, context.m_Registry);

		// Check that this is only one entity in the registry
		Comparison<size_t> numberOfEntities(1, registry.alive());
		COMPAIR_EQ(numberOfEntities);

		// Iterate over the registery and check for MockComponentWithContainers
		auto mockComponentView = registry.view<MockComponentWithContainers>();

		Comparison<std::vector<int>> intVector({ 2, 4, 8 });
		Comparison<std::vector<MockCustomDataType>> customTypeVector({ { 10.0, false }, {11.0, true} });

		for (auto [entity, mockComponent] : mockComponentView.each())
		{
			intVector.m_Actual = mockComponent.m_IntVector;
			customTypeVector.m_Actual = mockComponent.m_CustomTypeVector;
		}

		// Test!
		COMPAIR_EQ(intVector);
		COMPAIR_EQ(customTypeVector);
	}
*/
