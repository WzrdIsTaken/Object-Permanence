#include "EngineCore.h"

#include "MathUtils.h"

#include "DebugUtils.h"

namespace nabi::MathUtils
{
#ifdef USE_DEBUG_UTILS
	namespace
	{
		bool m_RandomSeeded = false;
	}
#endif // ifdef USE_DEBUG_UTILS

	void SeedRandom() NABI_NOEXCEPT
	{
#ifdef USE_DEBUG_UTILS
		ASSERT(!m_RandomSeeded, "Random is being seeded more than once!");
		m_RandomSeeded = true;
#endif // ifdef USE_DEBUG_UTILS

		srand(static_cast<unsigned int>(time(nullptr)));
	}

	int GenerateRandomNumber(int const max) NABI_NOEXCEPT
	{
		return (rand() % max) + 1;
	}

	int GenerateRandomNumberInRange(int const min, int const max) NABI_NOEXCEPT
	{
		ASSERT(min < max, "Trying to generate a random number in a range but [min] (" << min << ") is greater than or equal to [max] (" << max << ")!");
		return min + rand() % (max - min + 1);
	}
} // namespace nabi::MathUtils
