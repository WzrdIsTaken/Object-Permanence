#pragma once

// An example lib level file for game code

#include "Core.h"
// (see Core.h for why there is no lib level check, in the real game solution there would be a lib level check)

/*
* See docs/Quick Start Guide for an explanation of what the demo is doing.
* To run the demo, uncomment INCLUDE_DEMO in Defines.h and put this code in core:
* 
    #include "Demo\Core\Demo.h"

    nabi::NabiCoreSettings nabiCoreSettings = nabi::c_NabiCoreDefaultSettings;
    nabiCoreSettings.m_DataSettings.m_RouteDocument = "Nabi/Tests/Data/Demo/demo_route.xml";
    nabiCoreSettings.m_DataSettings.m_NabiCoreParseDocuments = nabi::DataSettings::NabiCoreParseMode::All;

    nabi::NabiMainParams const nabiParams =
    {
        nabiCoreSettings
    };

    nabi::WinMainParams const winParams =
    {
        hInstance, hPrevInstance, lpCmdLine, nShowCmd
    };

    int const result = nabi::Main<core::Demo>(nabiParams, winParams);
    return result;
*/

#ifdef INCLUDE_DEMO
	// anything else here if needed...
	// plus, in a real project we would have a "GameCore.h" which would be similar to this (eg, including "Core.h")
#endif // ifdef INCLUDE_DEMO
