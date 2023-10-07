#pragma once
#include "EngineCore.h"

#include "DirectXIncludes.h"

namespace nabi::Rendering
{
	struct Vertex
	{
		dx::XMFLOAT3 m_Position;
		dx::XMFLOAT2 m_UV;
		dx::XMFLOAT3 m_Normal;
	};
} // namespace nabi::Rendering
