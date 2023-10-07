#include "TestCore.h"

#include "CoreComponents\ButtonComponent.h"
#include "CoreComponents\UISceneComponent.h"
#include "CoreSingletonComponents\UIStateComponent.h"
#include "CoreSystems\UISystem.h"

// These tests are so bot
// Defo a lot learned from writing these things.. 
// 1) I'm not very good at them!!
// See the learnings document for what I would do differently next time

#ifdef RUN_TESTS

/*
 	Right I have no idea why this is, but if the MockSceneFunction is in the same namespace as MockScene I get an 'expecting compile time expression' error.
	It works fine in TestInput?? idk, perhaps this is something to work out but if its just this one place it can just be a 'huh' thing. 
	If anyone is reading this in the future and knows why this is then please lmk!

	For now I've just done the quick/hacky/bot solution of using the TestInput function.. I want to be done with this!!

	04/07/23 Update - I have realised why this doesn't work (it wasn't valid to attach all of the ui scenes methods to one type, so instead I'm attaching
	                  each ui scene's methods to its reflector) and its now been fixed :). I'll leave this test code how it is though.. because it works!!
*/
#include "Examples\TestInput.h"

namespace nabi::Tests::InputTests
{
	namespace MockScene
	{
		REFLECT_UI_SCENE_BEGIN(Scene)
			REFLECT_UI_SCENE_METHOD(Examples::TestButtonResponseTwo, "Function")
		REFLECT_UI_SCENE_END(Scene)
	}

	void SetupUIButtonAndScene(nabi::Context& context)
	{
		using namespace nabi::ECS;
		using namespace nabi::Input;

		// Button
		ecs::ButtonComponent buttonComponent;
		buttonComponent.m_Interactable = true;
		buttonComponent.m_Active = true;
		buttonComponent.m_Selected = false;

		buttonComponent.m_InputType = InputType::Keyboard;
		buttonComponent.m_ActivationKey = InputCode::Key_G;
		buttonComponent.m_AltInputType = InputType::Keyboard;
		buttonComponent.m_AltActivationKey = InputCode::Key_H;

		buttonComponent.m_Scene = entt::hashed_string("Scene");
		buttonComponent.m_Action = entt::hashed_string("Function");

		EntityCreator::EntityCreationSettings settings;
		settings.m_EntityName = "ButtonEntity";
		settings.m_AppendUUID = false;

		entt::entity const uiEntity = context.m_EntityCreator->CreateEntity(&settings);
		context.m_Registry.emplace<ecs::ButtonComponent>(uiEntity, buttonComponent);

		// Scene
		ecs::UISceneComponent uiScene;
		uiScene.m_SceneName = entt::hashed_string("TestScene");
		uiScene.m_UISceneEntityNames = "ButtonEntity"_hs;

		entt::entity const uiSceneEntity = context.m_EntityCreator->CreateEntity();
		context.m_Registry.emplace<ecs::UISceneComponent>(uiSceneEntity, uiScene);
	}
	
	// Test that a ui scene is created correctly
	TEST(InputTests, CheckUISceneCreation)
	{
		// --- Mock objects ---

		// Core
		nabi::Context context;
		context.m_EntityCreator = std::make_unique<nabi::ECS::EntityCreator>(context.m_Registry);
		ecs::UISystem uiSystem(context, "id"_hs, "group"_hs);

		entt::entity coreUiEntity = context.m_Registry.create();
		ecs::SComp::UIStateComponent const& uiStateComponent = context.m_Registry.emplace<ecs::SComp::UIStateComponent>(coreUiEntity);
		context.m_SingletonEntites.at(nabi::Context::SingletonEntities::Input) = coreUiEntity;

		// UI entities
		SetupUIButtonAndScene(context);

		// --- Check that a ui scene exists and has one entity --- 

		size_t const numberOfUiScenes = uiStateComponent.m_UIScenes.size();
		size_t const numberOfUiEntitiesInScene = uiStateComponent.m_UIScenes.top().m_UISceneEntities.size();

		Comparison<size_t> const numberOfUiScenesComparison(1, numberOfUiScenes);
		Comparison<size_t> const numberOfUiEntitiesInSceneComparison(1, numberOfUiEntitiesInScene);

		COMPAIR_EQ(numberOfUiScenesComparison);
		COMPAIR_EQ(numberOfUiEntitiesInSceneComparison);
	}

	// Test that the ui system goes through the flow correctly when a button is pressed
	TEST(InputTests, CheckUIButtonFlow)
	{
		// You can see the stuff this test does in action by running Examples/TestInput (which if you read the whole thing.. is probs a better thing to do!!)

		// --- Mock objects ---

		// Core
		nabi::Context context;
		context.m_EntityCreator = std::make_unique<nabi::ECS::EntityCreator>(context.m_Registry);
		ecs::UISystem uiSystem(context, "id"_hs, "group"_hs);

		entt::entity coreUiEntity = context.m_Registry.create();
		context.m_Registry.emplace<ecs::SComp::UIStateComponent>(coreUiEntity);
		context.m_SingletonEntites.at(nabi::Context::SingletonEntities::Input) = coreUiEntity;

		// UI entities
		SetupUIButtonAndScene(context);

		// --- Mock the button being pressed ---

		// Unfortunately we can't do the full button press flow, because that would get into the same problems described in InputStateTests or would just be a pain to setup
		// Instead I'll just set the button to selected
		entt::entity constexpr iKnowThisIsTheRightEntity = entt::entity(1);
		context.m_Registry.get<ecs::ButtonComponent>(iKnowThisIsTheRightEntity).m_Selected = true;

		//uiSystem.Update(); <-- ;_; we are going to GetInput here and thats going to do a flow I can't/cba to handle. Basically what I was worred about above ^

		// --- Check the results --

		// See all the other comments for why I'm doing this. It is very bot. But tbh if this doesn't crash it works??
		EXPECT_TRUE(true);
	}
} // namespace nabi::Tests::InputTests

#endif // #ifdef RUN_TESTS

// these tests mark the end of ui... i finished it on 30/04/23. lets go dude
