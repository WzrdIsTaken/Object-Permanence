#include "EngineCore.h"

#include "WinIncludes.h" // Context includes RenderCommand, which includes WinCore - so things get messy if we don't include it here

#include "Buffers\VertexBuffer.h"

#include "Buffers\RenderBuffers.h"
#include "Containers\Vertex.h"
#include "Context.h"

namespace nabi::Rendering
{
	VertexBuffer nabi::Rendering::VertexBufferLoader::operator()(RenderBufferData const& meshData, nabi::Context const& context) const NABI_NOEXCEPT
	{
		std::vector<Vertex> vertices;
		size_t const uvSize = meshData.m_Uvs.size();
		size_t const normalSize = meshData.m_Normals.size();

		for (size_t i = 0; i < meshData.m_Vertices.size(); ++i)
		{
			Vertex vertex;
			vertex.m_Position = meshData.m_Vertices[i];
			vertex.m_UV       = i < uvSize ? meshData.m_Uvs[i] : dx::XMFLOAT2();
			vertex.m_Normal   = i < normalSize ? meshData.m_Normals[i] : dx::XMFLOAT3();

			vertices.emplace_back(vertex);
		}

		VertexBuffer const vertexBuffer = context.m_RenderCommand->CreateVertexBuffer(vertices);
		return vertexBuffer;
	}
} // namespace nabi::Rendering