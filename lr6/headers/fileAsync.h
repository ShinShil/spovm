#pragma once
#include "main.h"
#include "debug.hpp"

extern "C" __declspec(dllexport) string read(string path);
extern "C" __declspec(dllexport) void write(string path, string src);
extern "C" __declspec(dllexport) void test();
