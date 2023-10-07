#pragma once
#include "TestCore.h"

// Examples are kinda like me testing stuff out, rather than an automated unit test ran on project startup

#ifdef RUN_TESTS

namespace nabi::Tests::Examples
{
	__interface IExample
	{
		bool Init()   { return false; }
		bool Update() { return false; }
		bool Render() { return false; }
	};
} // namespace nabi::Tests::Examples

#endif // ifdef RUN_TESTS
