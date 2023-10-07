#include "EngineCore.h"

#include "WinIncludes.h"

#include "Shaders\PixelShader.h"

#include "Context.h"

namespace nabi::Rendering
{
	TextureLoader::ResourceType TextureLoader::operator()(std::string const& resourcePath, nabi::Context const& context) const NABI_NOEXCEPT
	{
		Texture const texture = context.m_RenderCommand->CreateTexture(resourcePath);
		return std::make_shared<Texture>(texture);
	}
} // namespace nabi::Rendering
