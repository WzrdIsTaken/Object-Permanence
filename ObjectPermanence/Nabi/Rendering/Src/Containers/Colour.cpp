#include "EngineCore.h"

#include "Containers\Colour.h"

namespace nabi::Rendering
{
	Colour::Colour()
		: m_R(255)
		, m_G(255)
		, m_B(255)
		, m_A(255)
	{
	}

	Colour::Colour(int const r, int const g, int const b)
		: m_R(static_cast<unsigned char>(r))
		, m_G(static_cast<unsigned char>(g))
		, m_B(static_cast<unsigned char>(b))
		, m_A(static_cast<unsigned char>(255))
	{
	}

	Colour::Colour(int const r, int const g, int const b, int const a)
		: m_R(static_cast<unsigned char>(r))
		, m_G(static_cast<unsigned char>(g))
		, m_B(static_cast<unsigned char>(b))
		, m_A(static_cast<unsigned char>(a))
	{
	}

	Colour::Colour(float const r, float const g, float const b)
		: m_R(static_cast<unsigned char>(r * 255.0f))
		, m_G(static_cast<unsigned char>(g * 255.0f))
		, m_B(static_cast<unsigned char>(b * 255.0f)),
		m_A(255)
	{
	}

	Colour::Colour(float const r, float const g, float const b, float const a)
		: m_R(static_cast<unsigned char>(r * 255.0f))
		, m_G(static_cast<unsigned char>(g * 255.0f))
		, m_B(static_cast<unsigned char>(b * 255.0f))
		, m_A(static_cast<unsigned char>(a * 255.0f))
	{
	}

	Colour::Colour(unsigned char const r, unsigned char const g, unsigned char const b)
		: m_R(r)
		, m_G(g)
		, m_B(b)
		, m_A(255)
	{
	}

	Colour::Colour(unsigned char const r, unsigned char const g, unsigned char const b, unsigned char const a)
		: m_R(r)
		, m_G(g)
		, m_B(b)
		, m_A(a)
	{
	}

	Colour::operator dx::XMFLOAT3() const
	{
		dx::XMFLOAT3 float3;
		float3.x = static_cast<float>(m_R / 100);
		float3.y = static_cast<float>(m_G / 100);
		float3.z = static_cast<float>(m_B / 100);

		return float3;
	}
} // namespace nabi::Rendering
