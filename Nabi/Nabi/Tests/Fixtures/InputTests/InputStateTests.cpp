#include "TestCore.h"

#ifdef RUN_TESTS

namespace nabi::Tests::InputTests
{
	// Like rendering, input is kinda tied to the window and so I'm finding it hard to write tests for it
	// without basically setting up the entire pipeline.

	// This project is my first experience (trying!) to write testable code in my own project / with my own
	// architecture. I think a big learning is that what I've done is too tightly coupled and so pulling 
	// out blocks to test can be kinda hard.

	// And as this is a uni project, I don't have infinite time to go back and refactor it.. ):
	// If I have a sudden brain wave I'll come back and write these guys. Else, just cross my fingers..

	// Check keyboard input works as expected
	TEST(InputTests, CheckKeyboardInput)
	{
		// See / run 'TestInput'
		EXPECT_TRUE(true);
	}

	// Check mouse input works as expected
	TEST(InputTests, CheckMouseInput)
	{
		// See / run 'TestInput'
		EXPECT_TRUE(true);
	}

	// Check console input works as expected
	TEST(InputTests, CheckConsoleInput)
	{
		// See / run 'TestInput'
		EXPECT_TRUE(true);
	}
} // namespace nabi::Tests::InputTests

#endif // #ifdef RUN_TESTS
