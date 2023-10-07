#include "TestCore.h"

#include "Examples\TestPhysics.h"

#include "pugixml.hpp"

#include "CoreComponents\RigidbodyComponent.h"
#include "CoreComponents\ResourceComponents\ModelResourceComponent.h"
#include "CoreModules\InputModule.h"
#include "CoreModules\PhysicsModule.h"
#include "CoreModules\ReflectionModule.h"
#include "CoreSystems\LightingSystem.h"
#include "XmlParser.h"

#ifdef RUN_TESTS

namespace nabi::Tests::Examples
{
	ecs::ColliderComponent::ColliderMask constexpr c_PlayerMask  = 1 << 1;
	ecs::ColliderComponent::ColliderMask constexpr c_ObjectMask  = 1 << 1;
	ecs::ColliderComponent::ColliderMask constexpr c_RaycastMask = 1 << 2;

	typedef TestPhysics::TestPhysicsCollisionMasks TestPhysicsCollisionMasks;
	REFLECT_ENUM_UNDERLYING_BEGIN_DEFAULT(TestPhysicsCollisionMasks)
		REFLECT_ENUM_VALUE(TestPhysicsCollisionMasks::Object, "Object")
		REFLECT_ENUM_VALUE(TestPhysicsCollisionMasks::Player, "Player")
		REFLECT_ENUM_VALUE(TestPhysicsCollisionMasks::All,    "All")
	REFLECT_ENUM_END(TestPhysicsCollisionMasks)

	// --- Physics ---

	TestPhysics::TestPhysics(nabi::Context& context)
		: m_Context(context)
		, m_PlayerEntity(entt::null)
		, m_TestGameTime(nullptr)
	{
	}

	bool TestPhysics::Init()
	{
		// --- Systems ---

		// Having systems as ptrs gets around init order problems
		m_CollisionSystem = std::make_unique<ecs::CollisionSystem>(m_Context, "Collision"_hs, "NabiPhysicsTestSystems"_hs);
		m_InputSystem = std::make_unique<ecs::InputSystem>(m_Context, "Input"_hs, "NabiPhysicsTestSystems"_hs);
		m_PhysicsSystem = std::make_unique<ecs::PhysicsSystem>(m_Context, "Physics"_hs, "NabiPhysicsTestSystems"_hs);
		m_RenderSystem = std::make_unique<ecs::RenderSystem>(m_Context, "Render"_hs, "NabiPhysicsTestSystems"_hs);

		/*	
			Alright listen up. See ReflectionModule::c_EventConstraints
			I was just kinda dumb
		*/
		auto metaPhysics = entt::meta<TestPhysics>().type("TestPhysics"_hs)
			.func<&TestPhysics::TestCollisionEnterCallback>("TestCollisionEnterCallback"_hs)
			.func<&TestPhysics::TestCollisionExitCallback>("TestCollisionExitCallback"_hs)
			.func<&TestPhysics::TestVoidFunc>("Void"_hs);

		// --- Entities ---
		

		// Non xml loading
		auto result = m_Context.m_ThreadCommand->EnqueueTask("LoadEntities", MEDIUM_TASK, CRITICAL_PRIORITY, [&]() -> void
		{
			// Player Entity
			CollisionEntitySettings playerSettings = {};
			playerSettings.m_Position = { 0.0f, 0.0f, 0.0f };
			playerSettings.m_TexturePath = "Nabi/Tests/Data/Rendering/skybox_daybreak.png";
			playerSettings.m_CollisionMask = c_PlayerMask;
			playerSettings.m_ColliderType = ecs::ColliderComponent::ColliderType::Sphere;
			playerSettings.m_CollderInteractionType = ecs::ColliderComponent::InteractionType::Dynamic;
			playerSettings.m_ColliderSize = { 0.3f, 0.3f, 0.3f };
			playerSettings.m_GravityScale = 0.0f; // 1.0f
			m_PlayerEntity = CreateCollisionEntity(playerSettings);

			// - Collision Entities
			CollisionEntitySettings dynamicColliderSettings = {};
			dynamicColliderSettings.m_Position = { 1.0f, 0.0f, 0.0f }; // 0.0f, -1.0f, 0.0f
			dynamicColliderSettings.m_TexturePath = "Nabi/Tests/Data/Rendering/ball_texture.png";
			dynamicColliderSettings.m_CollisionMask = c_ObjectMask;
			dynamicColliderSettings.m_ColliderType = ecs::ColliderComponent::ColliderType::Cube;
			dynamicColliderSettings.m_CollderInteractionType = ecs::ColliderComponent::InteractionType::Dynamic; // Static
			dynamicColliderSettings.m_ColliderSize = { 0.625f, 0.625f, 0.625f }; // 10.0f, 0.625f, 0.625f
			dynamicColliderSettings.m_GravityScale = 0.0f;
			CreateCollisionEntity(dynamicColliderSettings);

			CollisionEntitySettings staticColliderSettings = dynamicColliderSettings;
			staticColliderSettings.m_CollderInteractionType = ecs::ColliderComponent::InteractionType::Static;
			staticColliderSettings.m_Position = { -1.0f, 0.0f, 0.0f };
			CreateCollisionEntity(staticColliderSettings);

			CollisionEntitySettings raycastableColliderSettings = staticColliderSettings;
			raycastableColliderSettings.m_Position = { 1.25f, 1.0f, 0.0f };
			raycastableColliderSettings.m_CollisionMask = c_RaycastMask;
			CreateCollisionEntity(raycastableColliderSettings);

			// --- Assets ---
			m_AssetBank = std::make_unique<SimpleAssetBank>(m_Context);
			m_AssetBank->LoadAssets();
		}
		); result.wait();

		// Multithreaded loading works fine (i think...)! Before or after NabiCore::Init. Just make sure to wait for the result, otherwise could get null data.

		/*
		* XML Loading
			CollisionEntitySettings playerSettings = {};
			playerSettings.m_Position = { 0.0f, 0.0f, 0.0f };
			playerSettings.m_TexturePath = "Nabi/Tests/Data/Rendering/skybox_daybreak.png";
			playerSettings.m_CollisionMask = c_PlayerMask;
			playerSettings.m_ColliderType = ecs::ColliderComponent::ColliderType::Sphere;
			playerSettings.m_CollderInteractionType = ecs::ColliderComponent::InteractionType::Dynamic;
			playerSettings.m_ColliderSize = { 0.3f, 0.3f, 0.3f };
			playerSettings.m_GravityScale = 0.0f; // 1.0f
			m_PlayerEntity = CreateCollisionEntity(playerSettings);

			nabi::Reflection::XmlParser xmlParser{};
			std::string const docPath = "Nabi/Tests/Data/Physics/test_physics_entities.xml";

			pugi::xml_document const doc = xmlParser.LoadDocument(docPath);
			xmlParser.ParseEntities(doc, m_Context.m_Registry);

			m_AssetBank = std::make_unique<SimpleAssetBank>(m_Context);
			m_AssetBank->LoadAssets();
		*/

		return true;
	}

	bool TestPhysics::Update()
	{
		if (m_PlayerEntity == entt::null)
		{
			//m_Context.m_Registry.destroy(m_PlayerEntity);
			//m_PlayerEntity = entt::null;

			return true;
		}

		using namespace ecs::InputModule;
		using namespace nabi::Input;

		ecs::RigidbodyComponent& playerEntityRigidbody = m_Context.m_Registry.get<ecs::RigidbodyComponent>(m_PlayerEntity);
		ecs::TransformComponent& playerEntityTransform = m_Context.m_Registry.get<ecs::TransformComponent>(m_PlayerEntity);

		float constexpr testEntitySpeed = 5.0f;
		float constexpr testEntityRotation = 0.1f;
		bool constexpr adjustRotation = false;

		InputState const resetKeyState = GetKeyboardKey(m_Context, InputCode::Key_Z);
		InputState const raycastKeyState = GetKeyboardKey(m_Context, InputCode::Key_R);

		InputState const wKeyState = GetKeyboardKey(m_Context, InputCode::Key_W);
		InputState const sKeyState = GetKeyboardKey(m_Context, InputCode::Key_S);
		InputState const aKeyState = GetKeyboardKey(m_Context, InputCode::Key_A);
		InputState const dKeyState = GetKeyboardKey(m_Context, InputCode::Key_D);
		InputState const qKeyState = GetKeyboardKey(m_Context, InputCode::Key_Q);
		InputState const eKeyState = GetKeyboardKey(m_Context, InputCode::Key_E);
		
		if (resetKeyState == InputState::Pressed)
		{
			ecs::TransformComponent& playerEntityTransformComponent = m_Context.m_Registry.get<ecs::TransformComponent>(m_PlayerEntity);
			dx::XMFLOAT3 constexpr resetValues = { 0.0f, 0.0f, 0.0f };

			playerEntityRigidbody.m_Velocity = resetValues;
			playerEntityRigidbody.m_AngularVelocity = resetValues;

			playerEntityTransformComponent.m_Position = resetValues;
			playerEntityTransformComponent.m_Rotation = resetValues;
		}
		if (raycastKeyState == InputState::Pressed)
		{
			ecs::PhysicsModule::RaycastSettings settings = ecs::PhysicsModule::c_DefaultRaycastSettings;
			settings.m_Mask = c_RaycastMask;
			//settings.m_Range = 0.5f;

			nabi::Physics::RaycastHitResult result = ecs::PhysicsModule::Raycast(
				m_Context,
				playerEntityTransform.m_Position,
				{ 1, 0, 0 }, // direction
				settings
			);

			LOG(LOG_PREP, LOG_INFO, LOG_CATEGORY_TEST, "Raycast " << (result ? "hit" : "didn't hit"), LOG_END);
		}

		if (wKeyState == InputState::Held)
		{
			playerEntityRigidbody.m_Velocity.y += testEntitySpeed * GetGameTime();
			if constexpr (adjustRotation) playerEntityRigidbody.m_AngularVelocity.x += testEntityRotation; 
		}
		if (sKeyState == InputState::Held)
		{
			playerEntityRigidbody.m_Velocity.y -= testEntitySpeed * GetGameTime();
			if constexpr (adjustRotation) playerEntityRigidbody.m_AngularVelocity.x -= testEntityRotation;
		}
		if (aKeyState == InputState::Held)
		{
			playerEntityRigidbody.m_Velocity.x -= testEntitySpeed * GetGameTime();
			if constexpr (adjustRotation) playerEntityRigidbody.m_AngularVelocity.y += testEntityRotation;
		}
		if (dKeyState == InputState::Held)
		{
			playerEntityRigidbody.m_Velocity.x += testEntitySpeed * GetGameTime();
			if constexpr (adjustRotation) playerEntityRigidbody.m_AngularVelocity.y -= testEntityRotation;
		}
		if (qKeyState == InputState::Held)
		{
			playerEntityRigidbody.m_Velocity.z -= testEntitySpeed * GetGameTime();
			if constexpr (adjustRotation) playerEntityRigidbody.m_AngularVelocity.x -= testEntityRotation;
		}
		if (eKeyState == InputState::Held)
		{
			playerEntityRigidbody.m_Velocity.z += testEntitySpeed * GetGameTime();
			if constexpr (adjustRotation) playerEntityRigidbody.m_AngularVelocity.x += testEntityRotation;
		}

		return true;
	}

	bool TestPhysics::Render()
	{
		return false;
	}

	void TestPhysics::SetGameTime(nabi::GameTime const* gametime)
	{
		m_TestGameTime = gametime;
	}

	entt::entity TestPhysics::CreateCollisionEntity(CollisionEntitySettings const& creationSettings) const
	{
		// Entity
		entt::entity const entity = m_Context.m_EntityCreator->CreateEntity();

		// Core
		ecs::TransformComponent transformComponent = {};
		transformComponent.m_Position = creationSettings.m_Position;
		transformComponent.m_Rotation = { 0, 0, 0 };
		transformComponent.m_Scale = { 0.25f, 0.25f, 0.25f };

		// Rendering
		ecs::RComp::ModelResourceComponent modelComponent = {};
		modelComponent.m_MeshPath = "PrimativeCube=1x1x1";
		modelComponent.m_TexturePath = creationSettings.m_TexturePath;
		modelComponent.m_PixelShaderPath = "Nabi/Rendering/Shaders/PixelShaderSkybox.cso";
		modelComponent.m_VertexShaderPath = "Nabi/Rendering/Shaders/VertexShaderSkybox.cso";

		// Physics
		ecs::RigidbodyComponent rigidbodyComponent = {};
		rigidbodyComponent.m_Drag = 1.0f;
		rigidbodyComponent.m_GravityScale = creationSettings.m_GravityScale;

		ecs::ColliderComponent colliderComponent = {};
		colliderComponent.m_ColliderType = creationSettings.m_ColliderType;
		colliderComponent.m_ColliderDimensions = creationSettings.m_ColliderSize;
		colliderComponent.m_Mask = creationSettings.m_CollisionMask;
		colliderComponent.m_InteractionType = creationSettings.m_CollderInteractionType;

		colliderComponent.m_OnCollisionEnterType = creationSettings.m_CollisionEnterType;
		colliderComponent.m_OnCollisionEnterAction = creationSettings.m_CollisionEnterFunc;
		colliderComponent.m_OnCollisionExitType = creationSettings.m_CollisionExitType;
		colliderComponent.m_OnCollisionExitAction = creationSettings.m_CollisionExitFunc;

		// Add everything!
		m_Context.m_Registry.emplace<ecs::TransformComponent>(entity, transformComponent);
		m_Context.m_Registry.emplace<ecs::RComp::ModelResourceComponent>(entity, modelComponent);
		m_Context.m_Registry.emplace<ecs::RigidbodyComponent>(entity, rigidbodyComponent);
		m_Context.m_Registry.emplace<ecs::ColliderComponent>(entity, colliderComponent);

		// Return
		return entity;
	}

	void TestPhysics::TestCollisionEnterCallback(nabi::Context& /*context*/, entt::entity const /*lhs*/, entt::entity const /*rhs*/)
	{
		LOG(LOG_PREP, LOG_TRACE, LOG_CATEGORY_TEST, "OnCollisionEnter!", LOG_END);
	}

	void TestPhysics::TestCollisionExitCallback(nabi::Context& /*context*/, entt::entity const /*lhs*/, entt::entity const /*rhs*/)
	{
		LOG(LOG_PREP, LOG_TRACE, LOG_CATEGORY_TEST, "OnCollisionExit!", LOG_END);
	}

	void TestPhysics::TestVoidFunc(nabi::Context& /*context*/, entt::entity const /*lhs*/, entt::entity const /*rhs*/)
	{
		// Actually won't see in Init for an explanation
		LOG(LOG_PREP, LOG_TRACE, LOG_CATEGORY_TEST, "Does something, hopefully", LOG_END);
	}

	float TestPhysics::GetGameTime() const
	{
		return m_TestGameTime ? static_cast<float>(m_TestGameTime->GetDeltaTime()) : 1.0f;
	}

	// --- Assets ---

	TestPhysics::SimpleAssetBank::SimpleAssetBank(nabi::Context& context)
		// Base
		: AssetBank(context)

		// Banks
		, m_RenderBufferBank(context)
		, m_PixelShaderBank(context)
		, m_VertexShaderBank(context)
		, m_TextureBank(context)
	{
	}

	TestPhysics::SimpleAssetBank::~SimpleAssetBank()
	{
		UnloadAssets();
	}

	bool TestPhysics::SimpleAssetBank::LoadAssets()
	{
		bool loadingSuccess = true;
		loadingSuccess &= Load3DModels();

		return loadingSuccess;
	}

	bool TestPhysics::SimpleAssetBank::UnloadAssets()
	{
		m_RenderBufferBank.Clear();
		m_PixelShaderBank.Clear();
		m_VertexShaderBank.Clear();
		m_TextureBank.Clear();

		return true;
	}

	bool TestPhysics::SimpleAssetBank::Load3DModels()
	{
		// (just took this straight from TestDraw)

		using namespace nabi::Rendering;
		using namespace nabi::Resource;

		// Set the banks for meshes
		VertexShaderLoader& vertexShaderLoader = m_VertexShaderBank.GetLoader();
		vertexShaderLoader.SetInputLayout(Layouts::c_MeshInputLayout);
		vertexShaderLoader.SetConstantBuffers({ ConstantBufferIndex::PerFrame, ConstantBufferIndex::PerRenderable });

		PixelShaderLoader& pixelShaderLoader = m_PixelShaderBank.GetLoader();
		pixelShaderLoader.SetConstantBuffers({ ConstantBufferIndex::PerLightChange });

		RenderBufferLoader& renderBufferLoader = m_RenderBufferBank.GetLoader();
		renderBufferLoader.SetLoadMode(RenderBufferLoader::LoadMode::_3D);

		// Iterate through all the entities with model components
		m_Context.m_Registry.view<ecs::RComp::ModelResourceComponent>()
			.each([&](entt::entity const entity, auto const& modelResourceComponent) -> void
				{
					// Mesh
					ResourceRef<Mesh> const meshResource = m_RenderBufferBank.LoadResource(modelResourceComponent.m_MeshPath);

					ecs::BufferComponent meshComponent = {};
					meshComponent.m_BufferResource = meshResource;

					// Shaders
					ResourceRef<PixelShader> const pixelShaderResource = m_PixelShaderBank.LoadResource(modelResourceComponent.m_PixelShaderPath);
					ResourceRef<VertexShader> const vertexShaderResource = m_VertexShaderBank.LoadResource(modelResourceComponent.m_VertexShaderPath);

					ecs::ShaderComponent shaderComponent = {};
					shaderComponent.m_PixelShaderResource = pixelShaderResource;
					shaderComponent.m_VertexShaderResource = vertexShaderResource;

					// Texture
					ResourceRef<Texture> const textureResource = m_TextureBank.LoadResource(modelResourceComponent.m_TexturePath);

					ecs::TextureComponent textureComponent = {};
					textureComponent.m_TextureResource = textureResource;

					// Assign the components to the registery
					m_Context.m_Registry.emplace_or_replace<ecs::BufferComponent>(entity, meshComponent);
					m_Context.m_Registry.emplace_or_replace<ecs::ShaderComponent>(entity, shaderComponent);
					m_Context.m_Registry.emplace_or_replace<ecs::TextureComponent>(entity, textureComponent);

					// Tag
					m_Context.m_Registry.emplace_or_replace<ecs::TComp::DrawPerspectiveTagComponent>(entity);
				});

		return true;
	}

} // namespace nabi::Tests::Examples

#endif // ifdef RUN_TESTS
