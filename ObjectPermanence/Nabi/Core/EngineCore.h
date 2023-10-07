#pragma once

// The Lib Level include file for all engine code.

// Nabi Core
#include "Config.h"
#include "Defines.h" // Including defines here just makes life easier. 

// Lib Level check
static_assert(LIB_LEVEL >= LIB_LEVEL_ENGINE);

/*
	I'll just wack this quick comment here in case any one (including future Ben) is wondering what LIB_LEVEL is.
	So... I planned for Nabi to be in its own project (like a game/engine project). However, when I was trying to
	set this up on 07/10/23 I just got so many errors (espically linker errors!). And when I finally got it to 
	work it didn't work right. Idk what was going on... I'll sure the process of creating a two projects /
	linking them up with packages etc is really easy for someone who knows what they are doing. But for me...
	it was well hard, and I just decided that it wasn't worth the effort. 

	):

	It is better for my time and sanity right now to just have everything in one project. Thanks Brandon for trying to help.

	LIB_LEVEL was going to be a per project define to make sure we don't include any, eg, game code in engine and create a dependency.
*/
