#pragma once
#include "GameCore.h"

// STD Headers
#include <functional>
#include <optional>

// Nabi Headers
#include "AssetBank.h"
#include "AudioEffect.h"
#include "Buffers\RenderBuffers.h"
#include "ResourceBank.h"
#include "Shaders\PixelShader.h"
#include "Shaders\VertexShader.h"
#include "Textures\Texture.h"
#include "TypeUtils.h"

// Game Headers
#include "Identifiers\AssetTypes.h"
#include "Typedefs\IntegerTypes.h"

/**
 * Creates and manages all of the game's assests. Eg models, audio effects, etc.
 * Kinda a big class, it could probs be decoupled a bit...
*/

namespace nabi
{
	struct Context;
} // namespace nabi

namespace core
{
	class GameAssetBank final : public nabi::Resource::AssetBank
	{
	public:
		GameAssetBank(nabi::Context& context);
		~GameAssetBank() override;

		bool LoadAssets(AssetType const assetType);
		bool UnloadAssets(AssetType const assetType);

		bool RefreshLoadedAssets(AssetType const assetType);

	private:
		DELETE_COPY_MOVE_CONSTRUCTORS(GameAssetBank)

		bool LoadModels();
		bool LoadSprites();
		bool LoadText();
		bool LoadAudio();

#pragma region Render Banks
		static size_t constexpr c_RenderBufferBankCapacity = 30u;
		static size_t constexpr c_RenderResourceBankCapacity = 5u;

		struct RenderablePaths final
		{
			std::string const& m_AssetPath;
			std::string const& m_VertexShaderPath;
			std::string const& m_PixelShaderPath;
			std::string const& m_TexturePath;
		};

		void SetRenderBankProperties(AssetType const renderableType);
		void LoadRenderable(RenderablePaths const& renderablePaths, entt::entity const entity,
			std::optional<std::function<void()>> preLoadOperation, nabi::Resource::ResourceCreationSettings const& renderBufferCreationSettings);
		[[nodiscard]] std::string CreateSpriteSheetResourceName(std::string const& filePath, std::string const& resourceName) const noexcept;

		nabi::Resource::ResourceBank<nabi::Rendering::RenderBuffers, nabi::Rendering::RenderBufferLoader, c_RenderBufferBankCapacity  > m_RenderBufferBank;
		nabi::Resource::ResourceBank<nabi::Rendering::VertexShader, nabi::Rendering::VertexShaderLoader, c_RenderResourceBankCapacity> m_VertexShaderBank;
		nabi::Resource::ResourceBank<nabi::Rendering::PixelShader, nabi::Rendering::PixelShaderLoader, c_RenderResourceBankCapacity> m_PixelShaderBank;
		nabi::Resource::ResourceBank<nabi::Rendering::Texture, nabi::Rendering::TextureLoader, c_RenderResourceBankCapacity> m_TextureBank;
#pragma endregion

#pragma region Audio Bank
		static size_t constexpr c_AudioBankCapacity = 5u;

		nabi::Resource::ResourceBank<nabi::Audio::AudioEffect, nabi::Audio::AudioEffectLoader, c_AudioBankCapacity> m_AudioEffectBank;
#pragma endregion
	};
} // namespace core
