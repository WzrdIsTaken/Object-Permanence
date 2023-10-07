#pragma once

#include "Defines.h"

#ifdef RUN_TESTS

namespace nabi::Tests
{
	// Why is this a marco? Tbh I just like the different colour compaired to the rest of the code and think it makes the tests asserts stand out
	// The main problem I wanted to solve was accidentally typing m_Expected/m_Actual twice, and these marcos do that still
	// Is it kinda just rewriting gtest macros? kinda.. but idk i had that typo and it cost me like 10 mins! never again xD 
	// I need to be able to trust the tests! not be double checking them...

#define COMPAIR_EQ(comparison) EXPECT_EQ(comparison.m_Expected, comparison.m_Actual);

	/// <summary>
	/// Basic comparison struct
	/// </summary>
	/// <typeparam name="T">The type to compair</typeparam>
	template<typename T>
	struct Comparison final
	{
		Comparison()
			: m_Expected{}
			, m_Actual{}
		{
		}

		explicit Comparison(T expected)
			: m_Expected(expected)
			, m_Actual{}
		{
		}

		Comparison(T expected, T actual)
			: m_Expected(expected)
			, m_Actual(actual)
		{
		}

		T m_Expected;
		T m_Actual;
	};
} // namespace nabi::Tests

#endif // #ifdef RUN_TESTS
