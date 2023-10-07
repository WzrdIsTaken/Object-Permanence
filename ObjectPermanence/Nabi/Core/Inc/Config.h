#pragma once

// Used in tandem with the ~Core.h files. Ensures that files from higher levels are not included in lower ones.
// The LIB_LEVEL macro is defined in configuration properties.

#define LIB_LEVEL_ENGINE 0
#define LIB_LEVEL_GAME   1
