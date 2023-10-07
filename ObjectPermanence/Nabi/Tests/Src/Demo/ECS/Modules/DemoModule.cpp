#include "Demo\DemoCore.h"

#ifdef INCLUDE_DEMO

#include "Demo\ECS\Modules\DemoModule.h"

namespace ecs::DemoModule
{
} // namespace ecs::DemoModule

#endif // ifdef INCLUDE_DEMO

/*
* How to use an entity property list + create an entity template:
* 
	void SpawnAsteroid(nabi::Context& context, dx::XMFLOAT3 const& spawnPosition)
	{
		nabi::ECS::EntityCreator::EntityCreationSettings asteriodCreationSettings = {};
		nabi::ECS::EntityPropertyList propertyList;

		std::string const spawnPositionAsString = nabi::Reflection::StringConverter::ToString<dx::XMFLOAT3>(spawnPosition);
		propertyList.AddProperty("TransformComponent", "Position", spawnPositionAsString);

		asteriodCreationSettings.m_EntityName = "Asteroid";
		asteriodCreationSettings.m_EntityTemplateName = "AsteroidEntityTemplate";
		asteriodCreationSettings.m_EntityOverriddenProperties = &propertyList;

		context.m_EntityCreator->CreateEntity(&asteriodCreationSettings);
	}
*/
