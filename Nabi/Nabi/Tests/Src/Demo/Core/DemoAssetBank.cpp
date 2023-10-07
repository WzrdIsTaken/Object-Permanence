#include "Demo\DemoCore.h"

#ifdef INCLUDE_DEMO

#include "Demo\Core\DemoAssetBank.h"

#include "CoreComponents\ResourceComponents\AudioResourceComponent.h"
#include "CoreComponents\ResourceComponents\ModelResourceComponent.h"
#include "CoreComponents\ResourceComponents\SpriteResourceComponent.h"
#include "CoreComponents\ResourceComponents\TextResourceComponent.h"
#include "CoreComponents\BufferComponent.h"
#include "CoreComponents\ShaderComponent.h"
#include "CoreComponents\TagComponents\DrawOrthographicTagComponent.h"
#include "CoreComponents\TagComponents\DrawPerspectiveTagComponent.h"
#include "CoreComponents\TextComponent.h"
#include "CoreComponents\TextureComponent.h"
#include "CoreComponents\TransformComponent.h"
#include "CoreModules\AudioModule.h"
#include "CoreModules\TextModule.h"
#include "StringStore.h"

namespace core
{
#define REMOVE_RESOURCE_COMPONENT_FROM_ENTITY
	
#ifdef REMOVE_RESOURCE_COMPONENT_FROM_ENTITY
	#define REMOVE_RESOURCE_COMPONENT_FROM_ENTITY_OPERATION(componentType) m_Context.m_Registry.remove<componentType>(entity);
#else
#define REMOVE_RESOURCE_COMPONENT_FROM_ENTITY_OPERATION NOT_DEFINED
#endif // ifdef REMOVE_RESOURCE_COMPONENT_FROM_ENTITY

	DemoAssetBank::DemoAssetBank(nabi::Context& context)
		: AssetBank(context)

		// Render Banks
		, m_RenderBufferBank(context)
		, m_VertexShaderBank(context)
		, m_PixelShaderBank(context)
		, m_TextureBank(context)

		// Audio Bank
		, m_AudioEffectBank(context)
	{
	}

	DemoAssetBank::~DemoAssetBank()
	{
		UnloadAssets();
	}

	bool DemoAssetBank::LoadAssets(AssetType const assetType)
	{
		bool loadingSuccessful = true;

		if (ValidAssetType(assetType, AssetType::Model))  loadingSuccessful &= LoadModels();
		if (ValidAssetType(assetType, AssetType::Sprite)) loadingSuccessful &= LoadSprites();
		if (ValidAssetType(assetType, AssetType::Text))   loadingSuccessful &= LoadText();
		if (ValidAssetType(assetType, AssetType::Audio))  loadingSuccessful &= LoadAudio();
		
		return loadingSuccessful;
	}

	bool DemoAssetBank::UnloadAssets()
	{
		m_RenderBufferBank.Clear();
		m_VertexShaderBank.Clear();
		m_PixelShaderBank.Clear();
		m_TextureBank.Clear();
		m_AudioEffectBank.Clear();

		return true;
	}

	bool DemoAssetBank::LoadModels()
	{
		SetRenderBankProperties(AssetType::Model);

		m_Context.m_Registry.view<ecs::RComp::ModelResourceComponent>()
			.each([&](entt::entity const entity, auto const& modelResourceComponent) -> void
				{
					RenderablePaths const modelPaths =
					{
						modelResourceComponent.m_MeshPath,
						modelResourceComponent.m_VertexShaderPath,
						modelResourceComponent.m_PixelShaderPath,
						modelResourceComponent.m_TexturePath
					};
					LoadRenderable(modelPaths, entity, std::nullopt, nabi::Resource::c_DefaultResourceCreationSettings);

					REMOVE_RESOURCE_COMPONENT_FROM_ENTITY_OPERATION(ecs::RComp::ModelResourceComponent);
				});

		return true;
	}

	bool DemoAssetBank::LoadSprites()
	{
		SetRenderBankProperties(AssetType::Sprite);
		
		m_Context.m_Registry.view<ecs::RComp::SpriteResourceComponent>()
			.each([&](entt::entity const entity, auto const& spriteResourceComponent) -> void
				{
					RenderablePaths const spritePaths =
					{
						spriteResourceComponent.m_ImagePath,
						spriteResourceComponent.m_VertexShaderPath,
						spriteResourceComponent.m_PixelShaderPath,
						spriteResourceComponent.m_ImagePath
					};
					LoadRenderable(spritePaths, entity,
						[&]() -> void
						{
							auto& renderBufferLoader = m_RenderBufferBank.GetLoader();
							renderBufferLoader.SetSpriteSheetProperties(spriteResourceComponent.m_UVs);
						}, 
						nabi::Resource::c_DefaultResourceCreationSettings);

					REMOVE_RESOURCE_COMPONENT_FROM_ENTITY_OPERATION(ecs::RComp::SpriteResourceComponent);
				});

		return true;
	}

	bool DemoAssetBank::LoadText()
	{
		SetRenderBankProperties(AssetType::Text);
		
		m_Context.m_Registry.view<ecs::TransformComponent const, ecs::RComp::TextResourceComponent>()
			.each([&](entt::entity const entity, auto& transformComponent, auto& textResourceComponent) -> void
				{
					int const textPoolSize = textResourceComponent.m_CharacterPoolSize;
					int const textContentLength = textResourceComponent.m_Content.length();

					// Add the text component to the base entity
					std::string const& textContent = nabi::Reflection::StringStore::Instance()->Add(textResourceComponent.m_Content,
						nabi::Reflection::StringStore::AddMode::CreateUnique); // if two bits of text have the same content, changing one shouldn't change the other

					ecs::TextComponent textComponent = {};
					textComponent.m_Content = entt::hashed_string(textContent.c_str());
					textComponent.m_Characters.reserve(static_cast<size_t>(textContentLength));
					textComponent.m_CharacterSpace    = textResourceComponent.m_CharacterSpace;
					textComponent.m_AsciiShift        = textResourceComponent.m_AsciiShift;
					textComponent.m_TextureAtlas      = textResourceComponent.m_TextureAtlas;
					textComponent.m_CharacterPoolSize = textResourceComponent.m_CharacterPoolSize;
					textComponent.m_ActiveInPool      = textContentLength;

					// Iterate through each character and load the sprite
					for (int i = 0; i < textPoolSize; ++i)
					{
						auto characterResourceCreationSettings = nabi::Resource::c_CreateUniqueDefaultCreationSettings;
						char character = ' ';

						if (i < textContentLength)
						{
							characterResourceCreationSettings = nabi::Resource::c_DefaultResourceCreationSettings;
							character = textResourceComponent.m_Content[i];
						}

						// Create the character entity
						entt::entity const characterEntity = m_Context.m_EntityCreator->CreateEntity();

						ecs::TransformComponent characterTransformComponent = transformComponent;
						characterTransformComponent.m_Position.x += textResourceComponent.m_CharacterSpace.x * static_cast<float>(i);
						characterTransformComponent.m_Position.y += textResourceComponent.m_CharacterSpace.y * static_cast<float>(i);
						m_Context.m_Registry.emplace<ecs::TransformComponent>(characterEntity, characterTransformComponent);

						if (m_Context.m_Registry.any_of<ecs::TComp::DrawOrthographicTagComponent>(entity))
						{
							m_Context.m_Registry.emplace<ecs::TComp::DrawOrthographicTagComponent>(characterEntity);
						}
						if (m_Context.m_Registry.any_of<ecs::TComp::DrawPerspectiveTagComponent>(entity))
						{
							m_Context.m_Registry.emplace<ecs::TComp::DrawPerspectiveTagComponent>(characterEntity);
						}

						// Create the character sprite
						std::string const assetPath = CreateSpriteSheetResourceName(textResourceComponent.m_FontPath, std::string(1u, character));
						RenderablePaths const textPaths =
						{
							assetPath,
							textResourceComponent.m_VertexShaderPath,
							textResourceComponent.m_PixelShaderPath,
							textResourceComponent.m_FontPath
						};
						LoadRenderable(textPaths, characterEntity,
							[&]() -> void
							{
								auto& renderBufferLoader = m_RenderBufferBank.GetLoader();
								auto const characterUVs = ecs::TextModule::CalculateCharacterUvs(m_Context, character,
									textResourceComponent.m_AsciiShift, textResourceComponent.m_TextureAtlas);

								renderBufferLoader.SetSpriteSheetProperties(characterUVs);
							}, 
							characterResourceCreationSettings);

						// Add the character to the text
						textComponent.m_Characters.push_back(characterEntity);
					}

					m_Context.m_Registry.emplace<ecs::TextComponent>(entity, textComponent);
					REMOVE_RESOURCE_COMPONENT_FROM_ENTITY_OPERATION(ecs::RComp::TextResourceComponent)
				});

		return true;
	}

	bool DemoAssetBank::LoadAudio()
	{
		m_Context.m_Registry.view<ecs::RComp::AudioResourceComponent const>()
			.each([&](entt::entity const entity, auto& audioResourceComponent) -> void
				{
					for (auto const [audioID, audioPath] : audioResourceComponent.m_Resources.Get())
					{
						nabi::Resource::ResourceRef<nabi::Audio::AudioEffect> audioResource = m_AudioEffectBank.LoadResource(audioPath);
						ecs::AudioModule::MapLoadedAudioEffectToID(m_Context, audioID, audioResource);
					}

					REMOVE_RESOURCE_COMPONENT_FROM_ENTITY_OPERATION(ecs::RComp::AudioResourceComponent)
				});

		return true;
	}

	void DemoAssetBank::SetRenderBankProperties(AssetType const renderableType)
	{
		using namespace nabi::Rendering;

		RenderBufferLoader& renderBufferLoader = m_RenderBufferBank.GetLoader();
		VertexShaderLoader& vertexShaderLoader = m_VertexShaderBank.GetLoader();
		PixelShaderLoader& pixelShaderLoader = m_PixelShaderBank.GetLoader();

		switch (renderableType)
		{
		case AssetType::Model:
			{
				renderBufferLoader.SetLoadMode(RenderBufferLoader::LoadMode::_3D);
				vertexShaderLoader.SetInputLayout(Layouts::c_MeshInputLayout);
				vertexShaderLoader.SetConstantBuffers({ ConstantBufferIndex::PerFrame, ConstantBufferIndex::PerRenderable });
				pixelShaderLoader.SetConstantBuffers({ ConstantBufferIndex::PerLightChange });
			}
			break;
		case AssetType::Sprite:
			[[fallthrough]]; // (Each text character is just a sprite)
		case AssetType::Text:
			{
				renderBufferLoader.SetLoadMode(RenderBufferLoader::LoadMode::_2D);
				vertexShaderLoader.SetInputLayout(Layouts::c_SpriteInputLayout);
				vertexShaderLoader.SetConstantBuffers({});
				pixelShaderLoader.SetConstantBuffers({});
			}
			break;
		default:
			ASSERT_FAIL("Using an unexpected RenderableType!");
			break;
		}
	}

	void DemoAssetBank::LoadRenderable(RenderablePaths const& renderablePaths, entt::entity const entity, 
		std::optional<std::function<void()>> preLoadOperation, nabi::Resource::ResourceCreationSettings const& renderBufferCreationSettings)
	{
		using namespace nabi::Rendering;
		using namespace nabi::Resource;

		ecs::BufferComponent bufferComponent = {};
		ecs::ShaderComponent shaderComponent = {};
		ecs::TextureComponent textureComponent = {};

		if (preLoadOperation)
		{
			(*preLoadOperation)();
		}

		// Asset
		ResourceRef<RenderBuffers> const renderBufferResource = m_RenderBufferBank.LoadResource(renderablePaths.m_AssetPath, renderBufferCreationSettings);
		bufferComponent.m_BufferResource = renderBufferResource;

		// Shaders
		ResourceRef<PixelShader> const pixelShaderResource = m_PixelShaderBank.LoadResource(renderablePaths.m_PixelShaderPath);
		ResourceRef<VertexShader> const vertexShaderResource = m_VertexShaderBank.LoadResource(renderablePaths.m_VertexShaderPath);
		shaderComponent.m_PixelShaderResource = pixelShaderResource;
		shaderComponent.m_VertexShaderResource = vertexShaderResource;

		// Texture
		ResourceRef<Texture> const textureResource = m_TextureBank.LoadResource(renderablePaths.m_TexturePath);
		textureComponent.m_TextureResource = textureResource;

		// Add components to entity
		m_Context.m_Registry.emplace_or_replace<ecs::BufferComponent>(entity, bufferComponent);
		m_Context.m_Registry.emplace_or_replace<ecs::ShaderComponent>(entity, shaderComponent);
		m_Context.m_Registry.emplace_or_replace<ecs::TextureComponent>(entity, textureComponent);
	}

	std::string DemoAssetBank::CreateSpriteSheetResourceName(std::string const& filePath, std::string const& resourceName) const noexcept
	{
		// This is because since resources are cached by name, if multiple sprites pulled from a sprite sheet use the same filepath,
		// then the resource will never be different as its just pulled from cache. This is important because the vertex buffer will 
		// be created with different UVs for each sprite. (aka, this is an easy hack)

		std::string const spriteSheetResourceName = filePath + "_" + resourceName;
		return spriteSheetResourceName;
	}
} // namespace core

#endif // ifdef INCLUDE_DEMO
