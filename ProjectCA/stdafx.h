#pragma once

#pragma comment(lib, "msimg32.lib")

#define _SILENCE_TR1_NAMESPACE_DEPRECATION_WARNING

//#define USING_DEBUG


#ifdef USING_DEBUG
	#ifdef UNICODE
		#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console") 
	#else
		#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console") 
	#endif
#endif

#include <Windows.h>
#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <unordered_map>
#include <memory>
#include <typeinfo>
#include	<chrono>
#include <functional>
#include <fstream>
#include <fmod.hpp>
#include "Types.h"
#include	"Include\Core\Math\CVector.h"
#include "Include\Event.h"
#include "Include\Core\Debuging\Debug.h"


#define SAFE_DELETE(ptr) if(ptr != nullptr){ delete ptr; ptr = nullptr; }
#define KEY_DOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)
#define KEY_UP(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 0 : 1)
#define KEY_PRESS(vk_code) ((GetAsyncKeyState(vk_code) & 0x8001) ? 1 : 0)
#define KEY_ONCE_PRESS(vk_code) ((GetAsyncKeyState(vk_code) & 0x0001) ? 1 : 0)