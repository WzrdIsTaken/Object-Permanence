#pragma once

// The Lib Level include file for all core game code. Also includes a number of headers to make life easier.
// 'core game code' refers to things which interface with the engine to perform core functionality - eg rendering. 
//  They are like pre-written building blocks to jump start whatever project I need to make. 

// Entt
namespace entt { inline namespace literals {} };
using namespace entt::literals;

// Nabi Core
#include "Config.h"
#include "Defines.h"

// Debug
#include "DebugUtils.h" // (includes Logger)

// Lib Level check
/*
*	So, the lib level is used to make sure eg game code isn't included in engine code. However, the static assert check 
*   only works if the, eg, game and engine code is in different solutions. I wrote a bunch of core[systems/components/etc]
*   to be used in the game code, but they live in the engine solution. Therefore, despite "Core.h" being the lib level include
*   for these files I can't enforce the check. When I make a game solution for a project, I will add the assert in a 'GameCore'. 
*   For now, just trust me that things are ok! I cba to improve this projects structure any more.. I've learned my lesson.. ;_;
*/

// A couple of other notes:
/*
*	1 - I'm not 100% sure about the entt namespace stuff I do here... Basically any time I use a hashed string, I want to use
*	    entt::literals so its convenient not to write it out all the time. But at the same time... it Core really the best
*	    place for this? 
*	2 - Also I'm not certain about globally including DebugUtils... but they are used pretty much everywhere so..?
*   
*	Regardless, these ~Core headers are so much better than they were. I don't think its worth doing any more...
*/
