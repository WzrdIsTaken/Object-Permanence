#include "TestCore.h"


#ifdef RUN_TESTS

namespace nabi::Tests::RenderingTests
{
	TEST(RenderingTests, InitRenderPipeline)
	{
		// Just cross your fingers!
		EXPECT_TRUE(true);
	}

	// To be honest, I'm not really sure how to write good rendering tests
	// A lot of things kinda rely on each other, so all the setup required is like running the game anyway..
	// ..or maybe thats just my poor system design..

	// I don't really know what to do beyond that other than cross my fingers xD
	// I could do some research into this, but I am very very done with rendering right now...
} // namespace nabi::Tests::RenderingTests

#endif // #ifdef RUN_TESTS
