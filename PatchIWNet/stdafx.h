#pragma once

#define WIN32_LEAN_AND_MEAN
#define _CRT_SECURE_NO_WARNINGS

#ifdef _DEBUG
#ifdef _MSC_FULL_VER
#if   _MSC_FULL_VER > 191125542
#pragma comment(lib, "libMinHook-x86-v141-mdd.lib")
#elif _MSC_FULL_VER > 190023918
#pragma comment(lib, "libMinHook-x86-v140-mdd.lib")
#endif
#endif
#else
#pragma comment(lib, "libMinHook-x86-v140-md.lib")
#endif

#include <Windows.h>
#include <tchar.h>

#include "Utility/General.h"
#include "Utility/Memory.h"
#include "Utility/Pattern.h"
#include "Utility/Logger.h"
#include "Utility/Config.h"

#include "Types.h"
