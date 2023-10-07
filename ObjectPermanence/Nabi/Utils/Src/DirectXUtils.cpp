#include "EngineCore.h"

#include "DirectXUtils.h"

#include <sstream>

namespace nabi::DirectXUtils
{
	std::string MatrixToString(dx::XMMATRIX const& matrix) NABI_NOEXCEPT
	{
		int constexpr rowLength    = 4;
		int constexpr collumLength = 4;

		dx::XMFLOAT4X4 matrixAsFloat4;
		dx::XMStoreFloat4x4(&matrixAsFloat4, matrix);

		std::ostringstream matrixStr;

		for (int row = 0; row < rowLength; ++row)
		{
			for (int collum = 0; collum < collumLength; ++collum)
			{
				matrixStr << matrixAsFloat4.m[row][collum];
				matrixStr << ", ";
			}
		}

		return matrixStr.str();
	}

	std::string Float3ToString(dx::XMFLOAT3 const& float3) NABI_NOEXCEPT
	{
		std::string const x = "x: " + std::to_string(float3.x);
		std::string const y = "y: " + std::to_string(float3.y);
		std::string const z = "z: " + std::to_string(float3.z);

		return x + " " + y + " " + z;
	}

	dx::XMFLOAT3 Float3Normalize(dx::XMFLOAT3 const& float3) NABI_NOEXCEPT
	{
		dx::XMVECTOR vector = dx::XMLoadFloat3(&float3);
		vector = dx::XMVector3Normalize(vector);

		dx::XMFLOAT3 normalizedVector;
		dx::XMStoreFloat3(&normalizedVector, vector);

		return normalizedVector;
	}

	dx::XMFLOAT3 Float3Rotate(dx::XMFLOAT3 const& float3, dx::XMFLOAT3 const& rotation) NABI_NOEXCEPT
	{
		dx::XMFLOAT3 result = c_Float3Zero;

		dx::XMVECTOR const float3Vector = dx::XMLoadFloat3(&float3);
		dx::XMVECTOR const rotationVector = dx::XMLoadFloat3(&rotation);

		dx::XMVECTOR const rotatedVector = dx::XMVector3Rotate(float3Vector, rotationVector);
		dx::XMStoreFloat3(&result, rotatedVector);

		return result;
	}
} // namespace nabi::DirectXUtils
