#pragma once
#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include <tchar.h>

#include <sstream>
#include <fstream>
#include <ctime>
#include <iostream>
#include <filesystem>
#include <mutex>

//DX
#include <d3d11.h>
#pragma comment(lib, "d3d11.lib")

//Vendor
#include "vendor/ImGui/imgui.h"
#include "vendor/ImGui/imgui_impl_win32.h"
#include "vendor/ImGui/imgui_impl_dx11.h"
#include "vendor/ImGui/imgui_internal.h"
//OpenCV
#include "opencv2/opencv.hpp"
#pragma comment(lib, "opencv_world4100.lib")
#pragma comment(lib, "opencv_world4100d.lib")
//Logger
#include "core/logger/logger.h"
using namespace Logger_c;

//core
#include "core/renderer/renderer.h"
#include "core/renderer/window.h"