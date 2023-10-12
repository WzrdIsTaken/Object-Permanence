/*
	                 ,;;,.                  ,;'',
                    /~\                    /~\
                   ([~])                  ([~])
                 ,_.~~~.                  .~~~.
               ()--|   ,\                /    ,\    ()
            ,_//   |   |>)              (<|   |\()--'m
         (~'  m''~)(   )/                \(   )   ~~'|
          \(~||~)/ //~\\                  //~\\     ||
             ||   ()   ()                ()   () /( || )\
             ||   ||   ||                ||   ||( '-||-' )
             || ,;.)   (.;,            ,;.)   (.;,(~\/~)/

	These are the header guards, protecting this project and keeping the lands of Nabi safe.
*/

// Nabi Headers
#include "Main.h"

// Game Headers
#include "Game.h"
#include "Typedefs\IntegerTypes.h"

s32 CALLBACK wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ s32 nShowCmd)
{
    nabi::NabiCoreSettings nabiCoreSettings = nabi::c_NabiCoreDefaultSettings;
    nabiCoreSettings.m_DataSettings.m_RouteDocument = "Game\\Data\\ECS\\route.xml";
    nabiCoreSettings.m_DataSettings.m_NabiCoreParseDocuments = nabi::DataSettings::NabiCoreParseMode::All;

    nabi::NabiMainParams const nabiParams =
    {
        nabiCoreSettings
    };

    nabi::WinMainParams const winParams =
    {
        hInstance, hPrevInstance, lpCmdLine, nShowCmd
    };

    s32 const result = nabi::Main<core::Game>(nabiParams, winParams);
    return result;
}
