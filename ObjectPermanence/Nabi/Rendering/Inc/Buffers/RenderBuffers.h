#pragma once
#include "EngineCore.h"

#include <memory>
#include <string>
#include <vector>

#include "DirectXIncludes.h"

#include "Buffers\IndexBuffer.h"
#include "Buffers\VertexBuffer.h"
#include "Containers\UVs.h"

namespace nabi
{
	struct Context;
} // namespace nabi
namespace objl
{
	struct Vector2;
	struct Vector3;
} // namespace objl

namespace nabi::Rendering
{
	// What nabi uses for rendering. 
	struct RenderBuffers final
	{
		IndexBuffer m_IndexBuffer;
		VertexBuffer m_VertexBuffer;
	};
	typedef RenderBuffers Mesh;
	typedef RenderBuffers Sprite;

	// What nabi uses to load a model or sprite
	struct RenderBufferData final
	{
		std::vector<dx::XMFLOAT3> m_Vertices;
		std::vector<dx::XMFLOAT3> m_Normals;
		std::vector<dx::XMFLOAT2> m_Uvs;
		std::vector<UINT> m_Triangles;
	};
	typedef RenderBufferData MeshData;
	typedef RenderBufferData SpriteData;

	class RenderBufferLoader final
	{
	public:
		typedef std::shared_ptr<RenderBuffers> ResourceType;

		enum class LoadMode : int
		{
			_3D,
			_2D,
			Undefined,
			ENUM_COUNT
		};

		[[nodiscard]] ResourceType operator()(std::string const& resourcePath, nabi::Context const& context) const NABI_NOEXCEPT;
		void SetLoadMode(LoadMode const loadMode) NABI_NOEXCEPT;
		void SetSpriteSheetProperties(UVs const uvs) NABI_NOEXCEPT;

	private:
		// 3D
		[[nodiscard]] std::shared_ptr<Mesh> Load3DMesh(std::string const& resourcePath, nabi::Context const& context) const NABI_NOEXCEPT;
		[[nodiscard]] ResourceType CheckAndLoadPrimative(nabi::Context const& context, std::string const& resourcePath) const NABI_NOEXCEPT;

		[[nodiscard]] dx::XMFLOAT2 ObjlVector2ToDxFloat2(objl::Vector2 const vector2) const NABI_NOEXCEPT;
		[[nodiscard]] dx::XMFLOAT3 ObjlVector3ToDxFloat3(objl::Vector3 const vector3) const NABI_NOEXCEPT;

		// 2D
		[[nodiscard]] std::shared_ptr<Sprite> Load2DSprite(std::string const& resourcePath, nabi::Context const& context) const NABI_NOEXCEPT;
		UVs m_SpriteUVs = { 1.0f, 1.0f, 0.0f, 0.0f };

		// General
		LoadMode m_LoadMode = LoadMode::Undefined;
	};
} // namespace nabi::Rendering
