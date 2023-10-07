#include "TestCore.h"

#include "pugixml.hpp"

#include "CoreComponents/EntityInfoComponent.h"
#include "EntityCreator.h"
#include "EntityPropertyList.h"
#include "MockObjects\MockComponent.h"
#include "XmlParser.h"

#ifdef RUN_TESTS

namespace nabi::Tests::ReflectionTests
{
	// Check creating a runtime entity works as expected
	TEST(ReflectionTests, CreateEntityAtRuntime)
	{
		// --- Setup ---

		using namespace nabi::ECS;
		using namespace nabi::Reflection;

		// Mock objects
		ComponentData mockComponentData;
		mockComponentData.m_Id = "MockComponent"_hs; // Important that this matches up with the reflected MockObjects/MockComponent

		EntityTemplateData mockEntityTemplateData;
		mockEntityTemplateData.m_Id = "MockEntityTemplate"_hs; // This can be anything for testing
		mockEntityTemplateData.m_Components.emplace_back(mockComponentData);

		entt::registry registry;
		EntityCreator::EntityTemplateStore entityStore;

		std::pair<std::string, EntityTemplateData> entityTemplatePair(mockEntityTemplateData.m_Id.data(), mockEntityTemplateData);
		entityStore.emplace(entityTemplatePair);

		EntityCreator entityCreator(registry);
		entityCreator.AssignEntityTemplateStore(std::move(entityStore));

		// --- Entity Creation ---

		// Create an entity! (fingers crossed...)
		EntityCreator::EntityCreationSettings entityCreationSettings1;
		entityCreationSettings1.m_EntityTemplateName = "MockEntityTemplate";
		entityCreator.CreateEntity(&entityCreationSettings1);

		// Create an entity with a custom name
		EntityCreator::EntityCreationSettings entityCreationSettings2;
		entityCreationSettings2.m_EntityTemplateName = "MockEntityTemplate";
		entityCreationSettings2.m_EntityName = "EntityWithCustomName";
		entityCreationSettings2.m_AppendUUID = false;
		entityCreator.CreateEntity(&entityCreationSettings2);

		// Create another entity, this time with a property override
		EntityCreator::EntityCreationSettings entityCreationSettings3;
		EntityPropertyList propertyList;
		propertyList.AddProperty("MockComponent", "IntType", "7"); // default value is 5

		entityCreationSettings3.m_EntityTemplateName = "MockEntityTemplate";
		entityCreationSettings3.m_EntityName = "EntityWithCustomNameAndOverriddenProperty";
		entityCreationSettings3.m_EntityOverriddenProperties = &propertyList;
		entityCreationSettings3.m_AppendUUID = false;

		entityCreator.CreateEntity(&entityCreationSettings3);

		// --- Tests ---

		// Check that one entity exists in the store
		Comparison<size_t> entityStoreSizeComparison(1, entityCreator.GetEntityStoreSize());
		COMPAIR_EQ(entityStoreSizeComparison);

		// Check that there are 3 entities in the registery
		Comparison<size_t> entitiesInRegisteryComparison(3, registry.size());
		COMPAIR_EQ(entitiesInRegisteryComparison);

		// Check that entities have the correct values
		Comparison<int> entityDefaultCreatedHasDefaultIntValue(5);
		Comparison<bool> entityWithCustomNameIsFound(true);
		Comparison<int> entityWithOverridenPropertyHasUpdatedIntValue(7);

		auto const mockComponentView = registry.view<ecs::EntityInfoComponent, MockComponent>();

		for (auto const [entity, entityInfo, mockComponent] : mockComponentView.each())
		{
			switch (entityInfo.m_EntityName)
			{
				case "Entity_0"_hs: // Created entities with a UUID added have there entity ID appended. The default name for an entity is "Entity"
					entityDefaultCreatedHasDefaultIntValue.m_Actual = mockComponent.m_IntType;
					break;
				case "EntityWithCustomName"_hs:
					entityWithCustomNameIsFound.m_Actual = true;
					break;
				case "EntityWithCustomNameAndOverriddenProperty"_hs:
					entityWithOverridenPropertyHasUpdatedIntValue.m_Actual = mockComponent.m_IntType;
					break;
				default:
					// The entity could not be found
					ASSERT_EQ(false, true);
					break;
			}
		}

		COMPAIR_EQ(entityDefaultCreatedHasDefaultIntValue);
		COMPAIR_EQ(entityWithCustomNameIsFound);
		COMPAIR_EQ(entityWithOverridenPropertyHasUpdatedIntValue);
	}

	// Check creating / destroying an entity group works correctly
	TEST(ReflectionTests, CreateEntityGroupAtRuntime)
	{
		// Just going to parse the entity template file here

		// --- Setup ---

		using namespace nabi::ECS;
		using namespace nabi::Reflection;

		// Mock objects
		entt::registry registry;
		std::string const docPath = "Nabi/Tests/Data/Reflection/test_entity_template_file.xml";

		// Deserialize data files
		nabi::Reflection::XmlParser xmlParser{};
		pugi::xml_document doc = xmlParser.LoadDocument(docPath);

		xmlParser.ParseEntities(doc, registry);

		// Set up of the entity template / group store
		EntityCreator::EntityTemplateStore entityTemplateStore;
		entityTemplateStore = xmlParser.GetEntityTemplateStore();

		EntityCreator::EntityGroupStore entityGroupStore;
		entityGroupStore = xmlParser.GetEntityGroupStore();

		EntityCreator entityCreator(registry);
		entityCreator.AssignEntityTemplateStore(std::move(entityTemplateStore));
		entityCreator.AssignEntityGroupStore(std::move(entityGroupStore));

		// Create an entity group
		entityCreator.CreateEntityGroup("TestTemplateGroup");

		// Check that the entity group store size is 1
		Comparison<size_t> entityGroupStoreSizeComparison(1u, entityCreator.GetEntityGroupStoreSize());
		COMPAIR_EQ(entityGroupStoreSizeComparison);

		// Check that there are 4 entities in the registery
		Comparison<size_t> registrySizeComparison(8u, registry.size()); // 8 as 4 from ParseEntities, 4 from the new entity group
		COMPAIR_EQ(registrySizeComparison);

		// Delete an entity group
		entityCreator.DestroyEntityGroup("TestTemplateGroup");

		// Check that there are 0 entities in the registry
		registrySizeComparison = { 0u, registry.alive() }; // 0 because all the entities in parse entities and here are in the same group (this will never be a problem in the real game)
		COMPAIR_EQ(registrySizeComparison);
	}
} // namespace nabi::Tests::ReflectionTests

#endif // #ifdef RUN_TESTS
