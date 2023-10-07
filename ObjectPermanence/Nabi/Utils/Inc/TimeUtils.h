#pragma once
#include "EngineCore.h"

#include <chrono>

namespace nabi::TimeUtils
{
	typedef std::chrono::steady_clock::time_point TimePoint;

	template<typename T>
	[[nodiscard]] T GetTimeDifference(TimePoint const& timePointOne, TimePoint const& timePointTwo) NABI_NOEXCEPT
	{
		auto const timeDifference = (timePointOne - timePointTwo).count();
		T const timeDifferenceAsT = static_cast<T>(timeDifference);

		return timeDifferenceAsT;
	}
} // namespace nabi::TimeUtils
