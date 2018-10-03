#pragma once

#pragma comment(lib, "msimg32.lib")

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
#include "Types.h"
#include	"Include\Core\Math\CVector.h"
#include "Include\Event.h"
#include "Include\Core\Debuging\Debug.h"


#define SAFE_DELETE(ptr) if(ptr != nullptr){ delete ptr; ptr = nullptr; }
#define KEY_DOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)