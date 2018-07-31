#pragma once


#include <Windows.h>
#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <unordered_map>
#include <memory>
#include "Types.h"
#include "Include\Core\Debuging\Debug.h"


#define SAFE_DELETE(ptr) if(ptr != nullptr){ delete ptr; ptr = nullptr; }
#define KEY_DOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)