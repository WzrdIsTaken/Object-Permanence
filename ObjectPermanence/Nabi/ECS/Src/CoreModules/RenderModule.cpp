#include "Core.h"

#include "CoreModules\RenderModule.h"

#include "Containers\Vertex.h"
#include "DirectXUtils.h"
#include "MathUtils.h"

namespace ecs::RenderModule
{
	nabi::Rendering::Mesh CreatePrimativeCube(nabi::Context const& context, float const width, float const height, float const depth)
	{
		// this function is a little jank but it works

		// Vertices
		std::vector<nabi::Rendering::Vertex> const cubeVertices = {
			// Bottom
			{ { -width, -height, -depth }, { 0.0f, 0.0f }, {} },
			{ { +width, -height, -depth }, { 1.0f, 0.0f }, {} },
			{ { -width, -height, +depth }, { 0.0f, 1.0f }, {} },
			{ { +width, -height, +depth }, { 1.0f, 1.0f }, {} },
			// Top
			{ { -width, +height, -depth }, { 0.0f, 0.0f }, {} },
			{ { +width, +height, -depth }, { 1.0f, 0.0f }, {} },
			{ { -width, +height, +depth }, { 0.0f, 1.0f }, {} },
			{ { +width, +height, +depth }, { 1.0f, 1.0f }, {} },
			// Front
			{ { -width, -height, +depth }, { 0.0f, 1.0f }, {} },
			{ { +width, -height, +depth }, { 1.0f, 1.0f }, {} },
			{ { -width, +height, +depth }, { 0.0f, 0.0f }, {} },
			{ { +width, +height, +depth }, { 1.0f, 0.0f }, {} },
			// Back
			{ { -width, -height, -depth }, { 1.0f, 1.0f }, {} },
			{ { +width, -height, -depth }, { 0.0f, 1.0f }, {} },
			{ { -width, +height, -depth }, { 1.0f, 0.0f }, {} },
			{ { +width, +height, -depth }, { 0.0f, 0.0f }, {} },
			// Left
			{ { -width, -height, +depth }, { 1.0f, 1.0f }, {} },
			{ { -width, +height, +depth }, { 1.0f, 0.0f }, {} },
			{ { -width, -height, -depth }, { 0.0f, 1.0f }, {} },
			{ { -width, +height, -depth }, { 0.0f, 0.0f }, {} },
			// Right
			{ { +width, -height, +depth }, { 0.0f, 1.0f }, {} },
			{ { +width, +height, +depth }, { 0.0f, 0.0f }, {} },
			{ { +width, -height, -depth }, { 1.0f, 1.0f }, {} },
			{ { +width, +height, -depth }, { 1.0f, 0.0f }, {} }
		};

		nabi::Rendering::VertexBuffer vertexBuffer = context.m_RenderCommand->CreateVertexBuffer(cubeVertices); // not const because shouldnt move constants

		// Indices
		std::vector<UINT> const cubeIndices = {
			0,2,1,    2,3,1,
			4,5,6,    6,5,7,
			8,10,9,   9,10,11,
			12,13,14, 13,15,14,
			16,18,17, 17,18,19,
			20,21,22, 22,21,23
		};

		nabi::Rendering::IndexBuffer indexBuffer = context.m_RenderCommand->CreateIndexBuffer(cubeIndices);     // " "

		// Create buffer
		nabi::Rendering::Mesh cube = {};
		cube.m_VertexBuffer = std::move(vertexBuffer);
		cube.m_IndexBuffer = std::move(indexBuffer);

		return cube;
	}

	nabi::Rendering::Mesh CreatePrimativeSphere(nabi::Context const& /*context*/, int const /*latLines*/, int const /*longLines*/)
	{
		// See https://www.braynzarsoft.net/viewtutorial/q16390-20-cube-mapping-skybox for an example

		FUNCTION_NOT_IMPLEMENTED
		return {};
	}
} // namespace ecs::RenderModule

