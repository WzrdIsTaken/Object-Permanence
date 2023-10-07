#include "Core.h"

#include "CoreModules\TextModule.h"

#include "CoreComponents\BufferComponent.h"
#include "CoreComponents\SpatialHierarchyComponent.h"
#include "CoreComponents\TextComponent.h"
#include "CoreComponents\TransformComponent.h"
#include "CoreModules\EntityModule.h"
#include "DirectXUtils.h"
#include "StringStore.h"

namespace ecs::TextModule
{
	namespace
	{
		TextComponent* const TryGetTextComponent(nabi::Context& context, entt::entity const textEntity)
		{
			TextComponent* textComponent = context.m_Registry.try_get<TextComponent>(textEntity);
			ASSERT(textComponent != nullptr, "Trying to do a text operation an entity which doesn't have a text component!");

			return textComponent;
		}
	}

	void UpdateTextContent(nabi::Context& context, entt::entity const textEntity, std::string const& newContent, bool const storeUpdatedStringInStore)
	{
		if (TextComponent* const textComponent = TryGetTextComponent(context, textEntity))
		{
			UpdateTextContent(context, *textComponent, newContent, storeUpdatedStringInStore);
		}
	}

	void UpdateTextContent(nabi::Context& context, TextComponent& textComponent, std::string const& newContent, bool const storeUpdatedStringInStore)
	{
		// Text is not dynamic (right now) for the following reason:
		// Basically we would need to re-create the vertex/index buffers because we can't just straight up clone an entity.
		// This is due to the way the resource caching works. If we clone a render buffer component, then the data it points 
		// to would be the same as the 'cloned from' object. Therefore when the uvs are updated both would change.
		// We are already doing two buffer copies in this operation and I think a pool is fine for now. 

		std::string const currentTextContent = textComponent.m_Content.data();
		int const currentTextContentLength = static_cast<int>(currentTextContent.length());
		int const newTextContentLength = static_cast<int>(newContent.length());
		int const textPoolSize = textComponent.m_CharacterPoolSize;

		ASSERT(newTextContentLength < textComponent.m_CharacterPoolSize,
			"Text's character pool (" << textComponent.m_CharacterPoolSize << ") doesn't support a string of size " << newTextContentLength << "! " <<
			"Excess characters will be cut.");

		// Update each character
		int characterIndex = 0;
		EntityModule::ForeachEntityChild(context, textComponent.m_Characters,
			[&](entt::entity const entity) -> bool
			{
				using namespace nabi::Rendering;

				if (characterIndex < textPoolSize)
				{
					char const newCharacter = characterIndex < newTextContentLength ? newContent.at(characterIndex) : ' ';
					char oldCharacter = '\0';
					if (characterIndex < currentTextContentLength)
					{
						oldCharacter = currentTextContent.at(characterIndex);
					}

					if (newCharacter != oldCharacter)
					{
						UVs const characterUvs = CalculateCharacterUvs(context, newCharacter, textComponent.m_AsciiShift, textComponent.m_TextureAtlas);

						BufferComponent const characterBufferComponent = context.m_Registry.get<BufferComponent>(entity);
						std::shared_ptr<RenderBuffers const> const characterBuffers = characterBufferComponent.m_BufferResource.GetResource();

						wrl::ComPtr<ID3D11Buffer> const characterVertexBuffer = characterBuffers->m_VertexBuffer.m_Buffer;
						UINT const characterVertexBufferSize = characterBuffers->m_VertexBuffer.m_ByteWidth;

						context.m_RenderCommand->UpdateBuffer(characterVertexBuffer, characterVertexBufferSize,
							[characterUvs, characterVertexBufferSize](D3D11_MAPPED_SUBRESOURCE& subresource) -> void
							{
								Vertex* const vertexData = reinterpret_cast<Vertex*>(subresource.pData);
								std::vector<Vertex> characterVertices = { vertexData, vertexData + characterVertexBufferSize };

								// We know text (under the hood, sprites) just have 4 uvs
								std::vector<dx::XMFLOAT2> const characterUvsAsFloat2 = UV::CreateSpriteUVs(characterUvs);
								characterVertices.at(0).m_UV = characterUvsAsFloat2.at(0);
								characterVertices.at(1).m_UV = characterUvsAsFloat2.at(1);
								characterVertices.at(2).m_UV = characterUvsAsFloat2.at(2);
								characterVertices.at(3).m_UV = characterUvsAsFloat2.at(3);

								memcpy(subresource.pData, characterVertices.data(), characterVertexBufferSize);
							});
					}

					++characterIndex;
					return true;
				}

				return false;
			});

		if (storeUpdatedStringInStore)
		{
			auto const& storedString = nabi::Reflection::StringStore::Instance()->Add(newContent, nabi::Reflection::StringStore::AddMode::CreateUnique);
			textComponent.m_Content = entt::hashed_string(storedString.c_str());
		}
		textComponent.m_ActiveInPool = characterIndex;
	}

	void CenterText(nabi::Context& context, entt::entity const textEntity, dx::XMFLOAT3 const* const position)
	{
		if (TextComponent* const textComponent = TryGetTextComponent(context, textEntity))
		{
			TransformComponent& transformComponent = context.m_Registry.get<TransformComponent>(textEntity);
			CenterText(context, *textComponent, transformComponent, position);
		}
	}

	void CenterText(nabi::Context& context, TextComponent const& textComponent, TransformComponent& textTransformComponent, dx::XMFLOAT3 const* const position)
	{
		using namespace nabi::DirectXUtils;

		// Set the texts position if its supplied
		if (position != nullptr)
		{
			textTransformComponent.m_Position = *position;
		}

		// Get the center point 
		dx::XMFLOAT3 const characterCenter = Float3Divide(textTransformComponent.m_Position, 2.0f);

		// Work out the first characters new position
		bool const evenNumberOfCharacters = textComponent.m_ActiveInPool % 2 == 0;
		float const halfLength = !evenNumberOfCharacters ? textComponent.m_ActiveInPool / 2 : (textComponent.m_ActiveInPool / 2) - 0.5f;

		dx::XMFLOAT2 const firstCharacterNewPositionHalf = Float2Multiply(textComponent.m_CharacterSpace, halfLength);
		dx::XMFLOAT3 const firstCharacterNewPosition = Float3Subtract(characterCenter, firstCharacterNewPositionHalf);

		// Iterate through each character and set its new position
		for (int i = 0; i < textComponent.m_ActiveInPool; ++i)
		{
			entt::entity const entity = textComponent.m_Characters[i];
			TransformComponent& transformComponent = context.m_Registry.get<TransformComponent>(entity);
			
			dx::XMFLOAT2 const characterSpace = Float2Multiply(textComponent.m_CharacterSpace, static_cast<float>(i));
			dx::XMFLOAT3 const newCharacterPosition = Float3Add(firstCharacterNewPosition, characterSpace);
			transformComponent.m_Position = newCharacterPosition;
		}
	}

	dx::XMFLOAT2 CalculateTextDimensions(nabi::Context& context, entt::entity const textEntity)
	{
		dx::XMFLOAT2 dimensions = { 0.0f, 0.0f };

		if (TextComponent* const textComponent = TryGetTextComponent(context, textEntity))
		{
			dimensions = CalculateTextDimensions(context, *textComponent);
		}

		return dimensions;
	}

	dx::XMFLOAT2 CalculateTextDimensions(nabi::Context& context, TextComponent const& textComponent)
	{
		// NOTE - This function currently only works for horizontal text!
		// I don't think there is enough infomation in the TextComponent right now to do anything else.

		dx::XMFLOAT2 dimensions = { 0.0f, 0.0f };
		dimensions.x = static_cast<FLOAT>(textComponent.m_TextureAtlas.x);
		dimensions.y = static_cast<FLOAT>(textComponent.m_TextureAtlas.y) * textComponent.m_ActiveInPool;

		/*
		This was for the sprite sheet, dummy
		SpatialHierarchyModule::ForeachEntityChild(context, textComponent.m_Characters,
			[&](entt::entity const entity) -> bool
			{
				TextureComponent const& characterTextureComponent = context.m_Registry.get<TextureComponent>(entity);
				std::shared_ptr<nabi::Rendering::Texture const> const characterTexture = characterTextureComponent.m_TextureResource.GetResource();

				dx::XMFLOAT2 const charcterTextureDims = context.m_RenderCommand->GetTextureDimensions(*characterTexture);
				dimensions.x =  charcterTextureDims.x;
				dimensions.y += charcterTextureDims.y;

				return true;
			});
		*/

		return dimensions;
	}

	std::vector<nabi::Rendering::UVs> CalculateStringUvs(nabi::Context const& context, std::string const& string, int const asciiShift, dx::XMINT2 const textureAtlas)
	{
		std::vector<nabi::Rendering::UVs> stringUvs = {};
		stringUvs.reserve(string.length());

		for (char const character : string)
		{
			nabi::Rendering::UVs const characterUv = CalculateCharacterUvs(context, character, asciiShift, textureAtlas);
			stringUvs.emplace_back(characterUv);
		}

		return stringUvs;
	}

	nabi::Rendering::UVs CalculateCharacterUvs(nabi::Context const& /*context*/, char const character, int const asciiShift, dx::XMINT2 const textureAtlas)
	{
		nabi::Rendering::UVs characterUvs = {};
		dx::XMFLOAT2 u, v;

		// We can think of the texture atlas as like a grid the sprites are on
		float const textureAtlasX = static_cast<float>(textureAtlas.x);
		float const textureAtlasY = static_cast<float>(textureAtlas.y);

		// Ok so this is the magic bit. The code is calculating the UV coordinates a character in the texture atlas. 
		// By subtracting 'asciiShift' from the char, we get the index of the character in the atlas
		// The reason for 1.0f/~ is to normalize the u between 0 and 1
		// The reason for the 'index %' is to get the result of textureAtlas.x / textureAtlas.x within the bounds of the sprite sheet
		int const index = static_cast<int>(character) - asciiShift;
		u = { 1.0f / textureAtlasX, 1.0f / textureAtlasY };
		v = { index % textureAtlas.x / textureAtlasX, (index / textureAtlas.x) / textureAtlasY };
		u.x += v.x;
		u.y += v.y;

		characterUvs.m_U1 = u.x; characterUvs.m_U2 = u.y;
		characterUvs.m_V1 = v.x; characterUvs.m_V2 = v.y;
		return characterUvs;
	}
} // namespace ecs::TextModule
