#pragma once
#include "Core.h"

#include "Containers\UVs.h"
#include "Context.h"

namespace ecs
{
	struct TextComponent;
	struct TransformComponent;
} // namespace ecs

namespace ecs::TextModule
{
	void UpdateTextContent(nabi::Context& context, entt::entity const textEntity, std::string const& newContent, bool const storeUpdatedStringInStore=false);
	void UpdateTextContent(nabi::Context& context, TextComponent& textComponent,  std::string const& newContent, bool const storeUpdatedStringInStore=false);

	void CenterText(nabi::Context& context, entt::entity const textEntity, dx::XMFLOAT3 const* const position = nullptr);
	void CenterText(nabi::Context& context, TextComponent const& textComponent, 
		TransformComponent& textTransformComponent, dx::XMFLOAT3 const* const position = nullptr);

	[[nodiscard]] dx::XMFLOAT2 CalculateTextDimensions(nabi::Context& context, entt::entity const textEntity);
	[[nodiscard]] dx::XMFLOAT2 CalculateTextDimensions(nabi::Context& context, TextComponent const& textComponent);

	[[nodiscard]] std::vector<nabi::Rendering::UVs> CalculateStringUvs(nabi::Context const& context, std::string const& string, 
		int const asciiShift, dx::XMINT2 const textureAtlas);
	[[nodiscard]] nabi::Rendering::UVs CalculateCharacterUvs(nabi::Context const& context, char const character, 
		int const asciiShift, dx::XMINT2 const textureAtlas);
} // namespace ecs::TextModule
