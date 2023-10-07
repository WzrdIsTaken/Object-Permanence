#include "TestCore.h"

#include "Examples\TestDraw.h"

#include "Containers\Colour.h"
#include "CoreComponents\LightComponent.h"
#include "CoreComponents\ResourceComponents\ModelResourceComponent.h"
#include "CoreComponents\ResourceComponents\SpriteResourceComponent.h"
#include "CoreComponents\ResourceComponents\TextResourceComponent.h"
#include "CoreComponents\SpatialHierarchyComponent.h"
#include "CoreComponents\TextComponent.h"
#include "CoreComponents\TransformComponent.h"
#include "CoreModules\TextModule.h"
#include "CoreSingletonComponents\LightStateComponent.h"
#include "GameTime.h"

#ifdef RUN_TESTS

namespace nabi::Tests::Examples
{
#pragma region Draw

	TestDraw::TestDraw(nabi::Context& context)
		: m_Context(context)
		, m_RenderSystem(context, "RenderId"_hs, "GroupId"_hs)
		, m_LightingSystem(context, "LightingId"_hs, "GroupId"_hs)
		, m_AssetBank(std::make_unique<TestAssetBank>(context))
	{
	}

	bool TestDraw::Init()
	{
#pragma region 3D
		{
			ecs::SComp::LightStateComponent& lightStateComponent = m_Context.m_Registry.get<ecs::SComp::LightStateComponent>(
				m_Context.m_SingletonEntites.at(nabi::Context::Graphic));
			lightStateComponent.m_LightCount = 1u;
#ifndef USE_EVENT_SYSTEM_UPDATE
			lightStateComponent.m_UpdateLights = true;
#else
			// So the reason this event was never firing was because the context was invalid at the point it was setup, therefore the registry was as well.
			// This should never be a problem in the actual game, as first NabiCore's constructor will create the context and then GameCore's init will create the systems.
			// Note the render call at the bottom of this region
			auto thisMakesTheLightingSystemEventFire = ecs::LightingSystem{ m_Context, "LightingId"_hs, "GroupId"_hs };
#endif // ifndef USE_EVENT_SYSTEM_UPDATE
			
			// --- Create the test entity ---
			entt::entity testEntity = m_Context.m_Registry.create();

			// Create a model component
			ecs::RComp::ModelResourceComponent modelComponent = {};
			modelComponent.m_MeshPath = "Nabi/Tests/Data/Rendering/ball_model.obj";
			modelComponent.m_TexturePath = "Nabi/Tests/Data/Rendering/ball_texture.png";
			modelComponent.m_PixelShaderPath = "Nabi/Rendering/Shaders/PixelShader3D.cso";
			modelComponent.m_VertexShaderPath = "Nabi/Rendering/Shaders/VertexShader3D.cso";

			// Create transform component
			ecs::TransformComponent transformComponent = {};
			transformComponent.m_Position = { 0, 0, 0 };
			transformComponent.m_Rotation = { 0, 0, 0 };
			transformComponent.m_Scale = { 1, 1, 1 };

			// Add the model component and a transform to the entity
			m_Context.m_Registry.emplace<ecs::RComp::ModelResourceComponent>(testEntity, modelComponent);
			m_Context.m_Registry.emplace<ecs::TransformComponent>(testEntity, transformComponent);

			// --- Create a light ---
			entt::entity lightEntity = m_Context.m_Registry.create();

			// Reuse the transform component
			transformComponent.m_Position = { 0, 1, 0 };

			// Create the light
			ecs::DirectionalLightComponent lightComponent;
			lightComponent.m_Colour = static_cast<dx::XMFLOAT3>(nabi::Rendering::Colours::White);
			lightComponent.m_Direction = { -10, 0, -20 };
			lightComponent.m_Intensity = 1.0f;

			lightComponent.m_AmbientIntensity = 1.0f;
			lightComponent.m_DiffuseIntensity = 1.0f;
			lightComponent.m_SpecularAttenuation = 1.0f;
			lightComponent.m_SpecularIntensity = 1.0f;

			// Add the light and transform component to the entity
			m_Context.m_Registry.emplace<ecs::DirectionalLightComponent>(lightEntity, lightComponent);
			m_Context.m_Registry.emplace<ecs::TransformComponent>(lightEntity, transformComponent);

#ifdef USE_EVENT_SYSTEM_UPDATE
			// Explanation for this above
			nabi::GameTime gameTime = {};
			thisMakesTheLightingSystemEventFire.Render(gameTime);
#endif // ifdef USE_EVENT_SYSTEM_UPDATE
		}
#pragma endregion

#pragma region 2D
		{
			// --- Create the test entity ---
			entt::entity testEntity = m_Context.m_Registry.create();

			// Create a sprite component
			ecs::RComp::SpriteResourceComponent spriteComponent = {};
			spriteComponent.m_ImagePath = "Nabi/Tests/Data/Rendering/sprite.png"; // font.png
			spriteComponent.m_PixelShaderPath = "Nabi/Rendering/Shaders/PixelShader2D.cso";
			spriteComponent.m_VertexShaderPath = "Nabi/Rendering/Shaders/VertexShader2D.cso";

			// Create transform component
			ecs::TransformComponent transformComponent = {};
			transformComponent.m_Position = { -1, 0, 0 };
			transformComponent.m_Rotation = { 0, 0, 0 };
			transformComponent.m_Scale = { 0.5, 0.5, 0.5 };

			// Add the sprite component and a transform to the entity
			m_Context.m_Registry.emplace<ecs::RComp::SpriteResourceComponent>(testEntity, spriteComponent);
			m_Context.m_Registry.emplace<ecs::TransformComponent>(testEntity, transformComponent);
		}
#pragma endregion

#pragma region Text
		{
			// --- Create the test entity ---
			entt::entity testEntity = m_Context.m_EntityCreator->CreateEntity(nullptr);

			// Create the text component
			ecs::RComp::TextResourceComponent textComponent;
			textComponent.m_FontPath = "Nabi/Tests/Data/Rendering/font.png";
			textComponent.m_PixelShaderPath = "Nabi/Rendering/Shaders/PixelShader2D.cso";
			textComponent.m_VertexShaderPath = "Nabi/Rendering/Shaders/VertexShader2D.cso";

			textComponent.m_Content = "blue";
			textComponent.m_CharacterSpace = { 0.38f, 0 };
			textComponent.m_AsciiShift = 32;
			textComponent.m_TextureAtlas = { 15, 8 };

			// Create transform component
			ecs::TransformComponent transformComponent = {};
			transformComponent.m_Position = { 0, 1, 0 };
			transformComponent.m_Rotation = { 0, 0, 0 };
			transformComponent.m_Scale = { 0.2f, 0.2f, 0.2f };

			// Add the text component and a transform to the entity
			m_Context.m_Registry.emplace<ecs::RComp::TextResourceComponent>(testEntity, textComponent);
			m_Context.m_Registry.emplace<ecs::TransformComponent>(testEntity, transformComponent);
		}
#pragma endregion

#pragma region Skybox
		// --- Create the test entity ---
		entt::entity testEntity = m_Context.m_Registry.create();

		// Create a model component
		ecs::RComp::ModelResourceComponent modelComponent = {};
		modelComponent.m_MeshPath = "PrimativeCube=15x15x15";
		modelComponent.m_TexturePath = "Nabi/Tests/Data/Rendering/skybox_daybreak.png";
		modelComponent.m_PixelShaderPath = "Nabi/Rendering/Shaders/PixelShaderSkybox.cso";
		modelComponent.m_VertexShaderPath = "Nabi/Rendering/Shaders/VertexShaderSkybox.cso";

		// Create transform component
		ecs::TransformComponent transformComponent = {};
		transformComponent.m_Position = { 0, 0, 0 }; //{ 1, -0.5f, 0 };
		transformComponent.m_Rotation = { 0, 0, 0 };
		transformComponent.m_Scale = { 0.25f, 0.25f, 0.25f }; // { 0.25f, 0.25f, 0.25f };

		// Add the model component and a transform to the entity
		m_Context.m_Registry.emplace<ecs::RComp::ModelResourceComponent>(testEntity, modelComponent);
		m_Context.m_Registry.emplace<ecs::TransformComponent>(testEntity, transformComponent);
#pragma endregion

		// Now.. just scene units..?

		// --- Load all assets ---
		m_AssetBank->LoadAssets();

		return true;
	}

	bool TestDraw::Update()
	{
		m_Context.m_Registry.view<ecs::TransformComponent, ecs::BufferComponent>()
			.each([&](entt::entity const entity, auto& transformComponent, auto const& modelComponent) -> void
				{
					float constexpr speed = 0.001f;
					//transformComponent.m_Rotation.x += speed;
					//transformComponent.m_Rotation.y += speed;
					//transformComponent.m_Rotation.z += speed;
				});

		return true;
	}

	bool TestDraw::Render()
	{
#ifndef USE_EVENT_SYSTEM_UPDATE
		nabi::GameTime gameTime = {};
		m_LightingSystem.Render(gameTime);
		m_RenderSystem.Render(gameTime);
#endif // ifndef USE_EVENT_SYSTEM_UPDATE

		return true;
	}

#pragma endregion

#pragma region Asset Bank

	TestDraw::TestAssetBank::TestAssetBank(nabi::Context& context)
		// Base
		: AssetBank(context)

		// Banks
		, m_RenderBufferBank(context)
		, m_PixelShaderBank(context)
		, m_VertexShaderBank(context)
		, m_TextureBank(context)
	{
	}

	TestDraw::TestAssetBank::~TestAssetBank()
	{
		UnloadAssets();
	}

	bool TestDraw::TestAssetBank::LoadAssets()
	{
		bool loadingSuccess = true;
		loadingSuccess &= Load3DModels();
		loadingSuccess &= Load2DSprites();
		loadingSuccess &= LoadText();

		return loadingSuccess;
	}

	bool TestDraw::TestAssetBank::UnloadAssets()
	{
		m_RenderBufferBank.Clear();
		m_PixelShaderBank.Clear();
		m_VertexShaderBank.Clear();
		m_TextureBank.Clear();

		return true;
	}

	bool TestDraw::TestAssetBank::Load3DModels()
	{
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

	bool TestDraw::TestAssetBank::Load2DSprites()
	{
		using namespace nabi::Rendering;
		using namespace nabi::Resource;

		// Set the banks for loading 2D sprites
		VertexShaderLoader& vertexShaderLoader = m_VertexShaderBank.GetLoader();
		vertexShaderLoader.SetInputLayout(Layouts::c_SpriteInputLayout);
		vertexShaderLoader.SetConstantBuffers({});

		PixelShaderLoader& pixelShaderLoader = m_PixelShaderBank.GetLoader();
		pixelShaderLoader.SetConstantBuffers({});

		RenderBufferLoader& renderBufferLoader = m_RenderBufferBank.GetLoader();
		renderBufferLoader.SetLoadMode(RenderBufferLoader::LoadMode::_2D);

		// Iterate through all the entities with sprite components
		m_Context.m_Registry.view<ecs::RComp::SpriteResourceComponent>()
			.each([&](entt::entity const entity, auto const& spriteResourceComponent) -> void
				{
					// Sprite
					renderBufferLoader.SetSpriteSheetProperties(spriteResourceComponent.m_UVs); // TODO - Do what text does with CreateSpriteSheetResourceName
					ResourceRef<Sprite> const spriteResource = m_RenderBufferBank.LoadResource(spriteResourceComponent.m_ImagePath);

					ecs::BufferComponent spriteComponent = {};
					spriteComponent.m_BufferResource = spriteResource;

					// Shaders
					ResourceRef<PixelShader> const pixelShaderResource = m_PixelShaderBank.LoadResource(spriteResourceComponent.m_PixelShaderPath);
					ResourceRef<VertexShader> const vertexShaderResource = m_VertexShaderBank.LoadResource(spriteResourceComponent.m_VertexShaderPath);

					ecs::ShaderComponent shaderComponent = {};
					shaderComponent.m_PixelShaderResource = pixelShaderResource;
					shaderComponent.m_VertexShaderResource = vertexShaderResource;

					// Texture
					ResourceRef<Texture> const textureResource = m_TextureBank.LoadResource(spriteResourceComponent.m_ImagePath);

					ecs::TextureComponent textureComponent = {};
					textureComponent.m_TextureResource = textureResource;

					// Assign the components to the registery
					m_Context.m_Registry.emplace_or_replace<ecs::BufferComponent>(entity, spriteComponent);
					m_Context.m_Registry.emplace_or_replace<ecs::ShaderComponent>(entity, shaderComponent);
					m_Context.m_Registry.emplace_or_replace<ecs::TextureComponent>(entity, textureComponent);

					// Tag
					m_Context.m_Registry.emplace_or_replace<ecs::TComp::DrawOrthographicTagComponent>(entity);

#pragma region Test get texture dims
					{
						dx::XMFLOAT2 const dims = m_Context.m_RenderCommand->GetTextureDimensions(*textureComponent.m_TextureResource.GetResource());
					}
#pragma endregion
				});

		return true;
	}

	bool TestDraw::TestAssetBank::LoadText()
	{
		using namespace nabi::Rendering;
		using namespace nabi::Resource;

		// Set the banks for loading 2D sprites
		VertexShaderLoader& vertexShaderLoader = m_VertexShaderBank.GetLoader();
		vertexShaderLoader.SetInputLayout(Layouts::c_SpriteInputLayout);
		vertexShaderLoader.SetConstantBuffers({});

		PixelShaderLoader& pixelShaderLoader = m_PixelShaderBank.GetLoader();
		pixelShaderLoader.SetConstantBuffers({});

		RenderBufferLoader& renderBufferLoader = m_RenderBufferBank.GetLoader();
		renderBufferLoader.SetLoadMode(RenderBufferLoader::LoadMode::_2D);

		m_Context.m_Registry.view<ecs::TransformComponent, ecs::RComp::TextResourceComponent>()
			.each([&](entt::entity const entity, auto const& transformComponent, auto const& textResourceComponent) -> void
				{
					int const textPoolSize = 16;
					int const textContentLength = textResourceComponent.m_Content.length();

					// Add the text component to the base entity
					ecs::TextComponent textComponentData = {};
					textComponentData.m_Content = entt::hashed_string(textResourceComponent.m_Content.c_str());
					textComponentData.m_Characters.reserve(static_cast<size_t>(textContentLength));
					textComponentData.m_CharacterSpace = textResourceComponent.m_CharacterSpace;
					textComponentData.m_AsciiShift = textResourceComponent.m_AsciiShift;
					textComponentData.m_TextureAtlas = textResourceComponent.m_TextureAtlas;
					textComponentData.m_CharacterPoolSize = textResourceComponent.m_CharacterPoolSize;
					textComponentData.m_ActiveInPool = textContentLength;

					ecs::TextComponent& textComponent = m_Context.m_Registry.emplace_or_replace<ecs::TextComponent>(entity, textComponentData);

					// Iterate through each character and load the sprite
					for (int i = 0; i < textPoolSize; ++i)
					{
						// Create the character creation settings if the character is used
						ResourceCreationSettings characterResourceCreationSettings = c_CreateUniqueDefaultCreationSettings;
						UVs characterUVs = {};
						char character = ' ';

						if (i < textContentLength)
						{	
							characterResourceCreationSettings = c_DefaultResourceCreationSettings;
							character = textResourceComponent.m_Content[i];	
						}

						// Load the character buffer
						characterUVs = ecs::TextModule::CalculateCharacterUvs(m_Context, character, textResourceComponent.m_AsciiShift, textResourceComponent.m_TextureAtlas);
						renderBufferLoader.SetSpriteSheetProperties(characterUVs);
						
						std::string resourceName = CreateSpriteSheetResourceName(textResourceComponent.m_FontPath, std::string(1, character));
						ResourceRef<Sprite> const characterResource = m_RenderBufferBank.LoadResource(resourceName, characterResourceCreationSettings);

						ecs::BufferComponent characterComponent = {};
						characterComponent.m_BufferResource = characterResource;

						// Grab the shader...
						ResourceRef<PixelShader> const pixelShaderResource = m_PixelShaderBank.LoadResource(textResourceComponent.m_PixelShaderPath);
						ResourceRef<VertexShader> const vertexShaderResource = m_VertexShaderBank.LoadResource(textResourceComponent.m_VertexShaderPath);

						ecs::ShaderComponent shaderComponent = {};
						shaderComponent.m_PixelShaderResource = pixelShaderResource;
						shaderComponent.m_VertexShaderResource = vertexShaderResource;

						// ...and the texture
						ResourceRef<Texture> const textureResource = m_TextureBank.LoadResource(textResourceComponent.m_FontPath);

						ecs::TextureComponent textureComponent = {};
						textureComponent.m_TextureResource = textureResource;

						// Create the character entity and assign the components to it
						entt::entity characterEntity = m_Context.m_EntityCreator->CreateEntity(nullptr);

						ecs::TransformComponent characterTransformComponent = transformComponent;
						characterTransformComponent.m_Position.x += textResourceComponent.m_CharacterSpace.x * static_cast<float>(i);
						characterTransformComponent.m_Position.y += textResourceComponent.m_CharacterSpace.y * static_cast<float>(i); 
						m_Context.m_Registry.emplace_or_replace<ecs::TransformComponent>(characterEntity, characterTransformComponent);
						m_Context.m_Registry.emplace_or_replace<ecs::BufferComponent>(characterEntity, characterComponent);
						m_Context.m_Registry.emplace_or_replace<ecs::ShaderComponent>(characterEntity, shaderComponent);
						m_Context.m_Registry.emplace_or_replace<ecs::TextureComponent>(characterEntity, textureComponent);

						// Tag
						m_Context.m_Registry.emplace_or_replace<ecs::TComp::DrawOrthographicTagComponent>(characterEntity);

						textComponent.m_Characters.push_back(characterEntity);
					}

					ecs::TextModule::UpdateTextContent(m_Context, entity, "lets go dude");
					dx::XMFLOAT3 pos = { 0, 2, 0 };
					ecs::TextModule::CenterText(m_Context, entity, &pos);

					entt::entity d = m_Context.m_EntityCreator->CreateEntity(nullptr);
					m_Context.m_EntityCreator->CloneEntity(entity);

#pragma region Test get text dims
					{
						dx::XMFLOAT2 const dims = ecs::TextModule::CalculateTextDimensions(m_Context, entity);
					}
#pragma endregion
				});

		return true;
	}

	std::string TestDraw::TestAssetBank::CreateSpriteSheetResourceName(std::string_view const filePath, std::string_view const resourceName) const
	{
		// This is because since resources are cached by name, if a multiple sprites pulled from a sprite sheet use the same filepath,
		// then the resource will never be different as its just pulled from cache. This is important because the vertex buffer will 
		// be created with different UVs for each sprite.

		std::string spriteSheetResourceName;
		spriteSheetResourceName += filePath;
		spriteSheetResourceName += "_";
		spriteSheetResourceName += resourceName;

		return spriteSheetResourceName;
	}

#pragma endregion
} // namespace nabi::Examples

#endif // #ifdef RUN_TESTS
