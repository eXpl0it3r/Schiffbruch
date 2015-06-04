#pragma once

#define ROUND(s) ((short) ((short)(s-0.5) == (short) s ? s+1:s)) //Zahlen runden Macro
#define INITGUID
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#define DIRECTINPUT_VERSION 0x0700
#define DIRECTDRAW_VERSION 0x0500

#include <windows.h>
#include <windowsx.h>
#include <ddraw.h>
#include <dinput.h>

#include "ddutil.h"
#include "resource.h"

#include "constants.hpp"
