#pragma once
#include "EngineCore.h"

#include "DirectXIncludes.h"

#include <string>

#define SAFE_RELEASE(obj) if ( (obj) != NULL    ) { (obj)->Release(); (obj) = NULL;    }
#define SAFE_DELETE(obj)  if ( (obj) != nullptr ) { delete (obj);     (obj) = nullptr; }

namespace nabi::DirectXUtils
{
	// am i being dumb or do these not exist in dx?? probs the former... i will write them as i need them

	// Debug
	[[nodiscard]] std::string MatrixToString(dx::XMMATRIX const& matrix) NABI_NOEXCEPT;
	[[nodiscard]] std::string Float3ToString(dx::XMFLOAT3 const& float3) NABI_NOEXCEPT;

	// Comparison
	[[nodiscard]] inline bool Float2Equal(dx::XMFLOAT2 const& float2Lhs, dx::XMFLOAT2 const& float2Rhs) NABI_NOEXCEPT
	{ return dx::XMVector2Equal(dx::XMLoadFloat2(&float2Lhs), dx::XMLoadFloat2(&float2Rhs)); }
	[[nodiscard]] inline bool Float3Equal(dx::XMFLOAT3 const& float3Lhs, dx::XMFLOAT3 const& float3Rhs) NABI_NOEXCEPT
	{ return dx::XMVector2Equal(dx::XMLoadFloat3(&float3Lhs), dx::XMLoadFloat3(&float3Rhs)); }
	[[nodiscard]] inline constexpr bool Int2Equal(dx::XMINT2 const& int2Lhs, dx::XMINT2 const& int2Rhs) NABI_NOEXCEPT
	{ return int2Lhs.x == int2Rhs.x && int2Lhs.y == int2Rhs.y; }
	[[nodiscard]] inline constexpr bool Int3Equal(dx::XMINT3 const& int3Lhs, dx::XMINT3 const& int3Rhs) NABI_NOEXCEPT
	{ return int3Lhs.x == int3Rhs.x && int3Lhs.y == int3Rhs.y && int3Lhs.z == int3Rhs.z; }

	[[nodiscard]] inline constexpr bool Float2Less(dx::XMFLOAT2 const& float2Lhs, dx::XMFLOAT2 const& float2Rhs) NABI_NOEXCEPT
	{ return float2Lhs.x < float2Rhs.x && float2Lhs.y < float2Rhs.y; }
	[[nodiscard]] inline constexpr bool Float3Less(dx::XMFLOAT3 const& float3Lhs, dx::XMFLOAT3 const& float3Rhs) NABI_NOEXCEPT
	{ return float3Lhs.x < float3Rhs.x && float3Lhs.y < float3Rhs.y && float3Lhs.z < float3Rhs.z; }

	[[nodiscard]] inline constexpr bool Float2Greater(dx::XMFLOAT2 const& float2Lhs, dx::XMFLOAT2 const& float2Rhs) NABI_NOEXCEPT
	{ return float2Lhs.x > float2Rhs.x && float2Lhs.y > float2Rhs.y; }
	[[nodiscard]] inline constexpr bool Float3Greater(dx::XMFLOAT3 const& float3Lhs, dx::XMFLOAT3 const& float3Rhs) NABI_NOEXCEPT
	{ return float3Lhs.x > float3Rhs.x && float3Lhs.y > float3Rhs.y && float3Lhs.z > float3Rhs.z; }

	[[nodiscard]] inline constexpr bool Float2LessOrEqual(dx::XMFLOAT2 const& float2Lhs, dx::XMFLOAT2 const& float2Rhs) NABI_NOEXCEPT
	{ return float2Lhs.x <= float2Rhs.x && float2Lhs.y <= float2Rhs.y; }
	[[nodiscard]] inline constexpr bool Float3LessOrEqual(dx::XMFLOAT3 const& float3Lhs, dx::XMFLOAT3 const& float3Rhs) NABI_NOEXCEPT
	{ return float3Lhs.x <= float3Rhs.x && float3Lhs.y <= float3Rhs.y && float3Lhs.z <= float3Rhs.z; }

	[[nodiscard]] inline constexpr bool Float2GreaterOrEqual(dx::XMFLOAT2 const& float2Lhs, dx::XMFLOAT2 const& float2Rhs) NABI_NOEXCEPT
	{ return float2Lhs.x >= float2Rhs.x && float2Lhs.y >= float2Rhs.y; }
	[[nodiscard]] inline constexpr bool Float3GreaterOrEqual(dx::XMFLOAT3 const& float3Lhs, dx::XMFLOAT3 const& float3Rhs) NABI_NOEXCEPT
	{ return float3Lhs.x >= float3Rhs.x && float3Lhs.y >= float3Rhs.y && float3Lhs.z >= float3Rhs.z; }

	// Addition
	[[nodiscard]] inline constexpr dx::XMFLOAT2 Float2Add(dx::XMFLOAT2 const& float2, float const scalar) NABI_NOEXCEPT
	{ return dx::XMFLOAT2(float2.x + scalar, float2.y + scalar); };
	[[nodiscard]] inline constexpr dx::XMFLOAT2 Float2Add(dx::XMFLOAT2 const& float2Lhs, dx::XMFLOAT2 const& float2Rhs) NABI_NOEXCEPT
	{ return dx::XMFLOAT2(float2Lhs.x + float2Rhs.x, float2Lhs.y + float2Rhs.y); };
	[[nodiscard]] inline constexpr dx::XMFLOAT2 Float2Add(dx::XMFLOAT2 const& float2Lhs, dx::XMFLOAT3 const& float3Rhs) NABI_NOEXCEPT
	{ return dx::XMFLOAT2(float2Lhs.x + float3Rhs.x, float2Lhs.y + float3Rhs.y); };
	[[nodiscard]] inline constexpr dx::XMFLOAT3 Float3Add(dx::XMFLOAT3 const& float3, float const scalar) NABI_NOEXCEPT
	{ return dx::XMFLOAT3(float3.x + scalar, float3.y + scalar, float3.z + scalar); };
	[[nodiscard]] inline constexpr dx::XMFLOAT3 Float3Add(dx::XMFLOAT3 const& float3Lhs, dx::XMFLOAT3 const& float3Rhs) NABI_NOEXCEPT
	{ return dx::XMFLOAT3(float3Lhs.x + float3Rhs.x, float3Lhs.y + float3Rhs.y, float3Lhs.z + float3Rhs.z); };
	[[nodiscard]] inline constexpr dx::XMFLOAT3 Float3Add(dx::XMFLOAT3 const& float3Lhs, dx::XMFLOAT2 const& float2Rhs) NABI_NOEXCEPT
	{ return dx::XMFLOAT3(float3Lhs.x + float2Rhs.x, float3Lhs.y + float2Rhs.y, float3Lhs.z + 0.0f); };

	// Subtraction
	[[nodiscard]] inline constexpr dx::XMFLOAT2 Float2Subtract(dx::XMFLOAT2 const& float2, float const scalar) NABI_NOEXCEPT
	{ return dx::XMFLOAT2(float2.x - scalar, float2.y - scalar); };
	[[nodiscard]] inline constexpr dx::XMFLOAT2 Float2Subtract(dx::XMFLOAT2 const& float2Lhs, dx::XMFLOAT2 const& float2Rhs) NABI_NOEXCEPT
	{ return dx::XMFLOAT2(float2Lhs.x - float2Rhs.x, float2Lhs.y - float2Rhs.y); };
	[[nodiscard]] inline constexpr dx::XMFLOAT2 Float2Subtract(dx::XMFLOAT2 const& float2Lhs, dx::XMFLOAT3 const& float3Rhs) NABI_NOEXCEPT
	{ return dx::XMFLOAT2(float2Lhs.x - float3Rhs.x, float2Lhs.y - float3Rhs.y); };
	[[nodiscard]] inline constexpr dx::XMFLOAT3 Float3Subtract(dx::XMFLOAT3 const& float3, float const scalar) NABI_NOEXCEPT
	{ return dx::XMFLOAT3(float3.x - scalar, float3.y - scalar, float3.z - scalar); };
	[[nodiscard]] inline constexpr dx::XMFLOAT3 Float3Subtract(dx::XMFLOAT3 const& float3Lhs, dx::XMFLOAT3 const& float3Rhs) NABI_NOEXCEPT
	{ return dx::XMFLOAT3(float3Lhs.x - float3Rhs.x, float3Lhs.y - float3Rhs.y, float3Lhs.z - float3Rhs.z); };
	[[nodiscard]] inline constexpr dx::XMFLOAT3 Float3Subtract(dx::XMFLOAT3 const& float3Lhs, dx::XMFLOAT2 const& float2Rhs) NABI_NOEXCEPT
	{ return dx::XMFLOAT3(float3Lhs.x - float2Rhs.x, float3Lhs.y - float2Rhs.y, float3Lhs.z - 0.0f ); };

	// Multiplication
	[[nodiscard]] inline constexpr dx::XMFLOAT2 Float2Multiply(dx::XMFLOAT2 const& float2, float const scalar) NABI_NOEXCEPT
	{ return dx::XMFLOAT2(float2.x * scalar, float2.y * scalar); };
	[[nodiscard]] inline constexpr dx::XMFLOAT2 Float2Multiply(dx::XMFLOAT2 const& float2Lhs, dx::XMFLOAT2 const& float2Rhs) NABI_NOEXCEPT
	{ return dx::XMFLOAT2(float2Lhs.x * float2Rhs.x, float2Lhs.y * float2Rhs.y); };
	[[nodiscard]] inline constexpr dx::XMFLOAT2 Float2Multiply(dx::XMFLOAT2 const& float2Lhs, dx::XMFLOAT3 const& float3Rhs) NABI_NOEXCEPT
	{ return dx::XMFLOAT2(float2Lhs.x * float3Rhs.x, float2Lhs.y * float3Rhs.y); };
	[[nodiscard]] inline constexpr dx::XMFLOAT3 Float3Multiply(dx::XMFLOAT3 const& float3, float const scalar) NABI_NOEXCEPT
	{ return dx::XMFLOAT3(float3.x * scalar, float3.y * scalar, float3.z * scalar); };
	[[nodiscard]] inline constexpr dx::XMFLOAT3 Float3Multiply(dx::XMFLOAT3 const& float3Lhs, dx::XMFLOAT3 const& float3Rhs) NABI_NOEXCEPT
	{ return dx::XMFLOAT3(float3Lhs.x * float3Rhs.x, float3Lhs.y * float3Rhs.y, float3Lhs.z * float3Rhs.z); };
	[[nodiscard]] inline constexpr dx::XMFLOAT3 Float3Multiply(dx::XMFLOAT3 const& float3Lhs, dx::XMFLOAT2 const& float2Rhs) NABI_NOEXCEPT
	{ return dx::XMFLOAT3(float3Lhs.x * float2Rhs.x, float3Lhs.y * float2Rhs.y, float3Lhs.z * 1.0f); };

	// Division
	[[nodiscard]] inline constexpr dx::XMFLOAT2 Float2Divide(dx::XMFLOAT2 const& float2, float const scalar) NABI_NOEXCEPT
	{ return dx::XMFLOAT2(float2.x / scalar, float2.y / scalar); };
	[[nodiscard]] inline constexpr dx::XMFLOAT2 Float2Divide(dx::XMFLOAT2 const& float2Lhs, dx::XMFLOAT2 const& float2Rhs) NABI_NOEXCEPT
	{ return dx::XMFLOAT2(float2Lhs.x / float2Rhs.x, float2Lhs.y / float2Rhs.y); };
	[[nodiscard]] inline constexpr dx::XMFLOAT2 Float2Divide(dx::XMFLOAT2 const& float2Lhs, dx::XMFLOAT3 const& float3Rhs) NABI_NOEXCEPT
	{ return dx::XMFLOAT2(float2Lhs.x / float3Rhs.x, float2Lhs.y / float3Rhs.y); };
	[[nodiscard]] inline constexpr dx::XMFLOAT3 Float3Divide(dx::XMFLOAT3 const& float3, float const scalar) NABI_NOEXCEPT
	{ return dx::XMFLOAT3(float3.x / scalar, float3.y / scalar, float3.z / scalar); };
	[[nodiscard]] inline constexpr dx::XMFLOAT3 Float3Divide(dx::XMFLOAT3 const& float3Lhs, dx::XMFLOAT3 const& float3Rhs) NABI_NOEXCEPT
	{ return dx::XMFLOAT3(float3Lhs.x / float3Rhs.x, float3Lhs.y / float3Rhs.y, float3Lhs.z / float3Rhs.z); };
	[[nodiscard]] inline constexpr dx::XMFLOAT3 Float3Divide(dx::XMFLOAT3 const& float3Lhs, dx::XMFLOAT2 const& float2Rhs) NABI_NOEXCEPT
	{ return dx::XMFLOAT3(float3Lhs.x / float2Rhs.x, float3Lhs.y / float2Rhs.y, float3Lhs.z / 1.0f ); };

	// Squaring
	[[nodiscard]] inline constexpr dx::XMFLOAT2 Float2Square(dx::XMFLOAT2 const& float2) NABI_NOEXCEPT
	{ return dx::XMFLOAT2(float2.x * float2.x, float2.y * float2.y); }
	[[nodiscard]] inline constexpr dx::XMFLOAT3 Float3Square(dx::XMFLOAT3 const& float3) NABI_NOEXCEPT
	{ return dx::XMFLOAT3(float3.x * float3.x, float3.y * float3.y, float3.z * float3.z); }

	// Vector
	[[nodiscard]] dx::XMFLOAT3 Float3Normalize(dx::XMFLOAT3 const& float3) NABI_NOEXCEPT;
	[[nodiscard]] dx::XMFLOAT3 Float3Rotate(dx::XMFLOAT3 const& float3, dx::XMFLOAT3 const& rotation) NABI_NOEXCEPT;

	// Constants
	dx::XMINT2 constexpr c_Int2Zero = { 0, 0 };
	dx::XMINT3 constexpr c_Int3Zero = { 0, 0, 0 };
	dx::XMFLOAT2 constexpr c_Float2Zero = { 0.0f, 0.0f };
	dx::XMFLOAT3 constexpr c_Float3Zero = { 0.0f, 0.0f, 0.0f };
} // namespace nabi::DirectXUtils
