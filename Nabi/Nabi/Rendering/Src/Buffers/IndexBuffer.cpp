#include "EngineCore.h"

#include "WinIncludes.h" // Context includes RenderCommand, which includes WinCore - so things get messy if we don't include it here

#include "Buffers\IndexBuffer.h"

#include "Buffers\RenderBuffers.h"
#include "Context.h"

namespace nabi::Rendering
{
	IndexBuffer nabi::Rendering::IndexBufferLoader::operator()(RenderBufferData const& meshData, nabi::Context const& context) const NABI_NOEXCEPT
	{
		IndexBuffer const indexBuffer = context.m_RenderCommand->CreateIndexBuffer(meshData.m_Triangles);
		return indexBuffer;
	}
} // namespace nabi::Rendering
