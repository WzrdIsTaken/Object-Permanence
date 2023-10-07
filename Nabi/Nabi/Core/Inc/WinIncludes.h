#pragma once

// Thank you ChiliTomatoNoodle for your amazing tutorial series
// And this was a great video as well - https://www.youtube.com/watch?v=5W7JLgFCkwI. Ty The Cherno
// And tyvm Sam + Brandon

// Target Windows 8 or later (was previously win7 (0x0601) but xaudio is windows 8+)
#define _WIN32_WINNT 0x0602
#include <sdkddkver.h>

// Disable any Windows stuff we don't want
#define WIN32_LEAN_AND_MEAN

#define NOATOM
#define NOCLIPBOARD
#define NOCOLOR
#define NOCOMM
#define NOCTLMGR
#define NODEFERWINDOWPOS
#define NODRAWTEXT
#define NOGDICAPMASKS
#define NOHELP
#define NOICONS
#define NOIMAGE
#define NOKANJI
#define NOKERNEL
#define NOMCX
#define NOMEMMGR
#define NOMENUS
#define NOMETAFILE
#define NOMINMAX
#define NONLS
#define NOOPENFILE
#define NOPROFILER
#define NOPROXYSTUB
#define NORASTEROPS
#define NORPC
#define NOSCROLL
#define NOSERVICE
#define NOSOUND
#define NOSYSCOMMANDS
#define NOSYSMETRICS
#define NOTAPE
#define NOTEXTMETRIC
#define NOWH

#define OEMRESOURCE
#define STRICT

// Finally, include Windows and get ready to rock
#include <windows.h>
