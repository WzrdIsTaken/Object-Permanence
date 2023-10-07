#include "Demo\DemoCore.h"

#ifdef INCLUDE_DEMO

#include "Demo\UI\DemoUIScene.h"

#include "ReflectionIncludes.h"

#include "Context.h"
#include "CoreComponents\TextComponent.h"
#include "CoreModules\CameraModule.h"
#include "CoreModules\TextModule.h"
#include "CoreModules\UIModule.h"

namespace ui::DemoUIScene
{
	REFLECT_UI_SCENE_BEGIN(DemoScene)
		REFLECT_UI_SCENE_METHOD_DEFAULT(ButtonResponse)
	REFLECT_UI_SCENE_END(DemoScene)

	void ButtonResponse(nabi::Context& context, entt::entity const uiEntity)
	{
		bool const clickInsideElement = true; //ecs::UIModule::CheckIfMouseIsOverElement(context, CameraModule::GetMainPerspectiveCameraComponent(context), uiEntity); (wip)
		if (clickInsideElement)
		{
			// There is only one text component in the scene, so we can do this.
			// If this isn't an option.. 2 things I can think of off the top of my head:
			// 1 - Reflect the spatial hierachy component's children property and add the text entity to it. 
			// 2 - Find it with FindFirstEntityByName
			// Solution 2 is a lot worse than solution 1, but this problem might never actually exist so I'm not going to worry about it rn!

			context.m_Registry.view<ecs::TextComponent>().each( 
				[&](auto& textComponent) -> void
				{
					static int count = 0;
					++count;

					int const nonStaticCount = count;
					context.m_ThreadCommand->PushTaskToTaskTaskQueue(STRINGIFY(CORE_TASK_RENDER),
						[&context, &textComponent, nonStaticCount]() -> void
						{
							ecs::TextModule::UpdateTextContent(context, textComponent, std::to_string(nonStaticCount));
						}
					);
				});
		}
	}
} // namesapce ui::DemoUIScene

#endif // ifdef INCLUDE_DEMO
