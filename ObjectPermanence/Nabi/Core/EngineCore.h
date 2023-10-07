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

	The steps I followed to set up the solution where:
		- Create a new repo and clone it
		- Copy/paste the Nabi code folder and solution file in (to keep all of Nabi's project settings)
			- Though now you can probs just copy/paste this project's code/solution and delete all the 'Game' code.
			  This will save you having to add Nabi\ infront of $(ProjectDir) in additional include directories,
			  chaning the shader output directories and data paths (eg Tests/ -> Nabi/Tests ->)
		- You then may or may not have to remove the code files and add them back in using VS.
		- Follow the steps described here https://stackoverflow.com/a/22356663/8890269 to rename the solution and project files.
		
		- There is almost certainly better ways of doing this, but its what worked for me.
*/
