#pragma once
#include <cstdint>
class HackManager
{
public:
	// main entry point
	static void Loop();

	// getter methods 
	static void RunFeatures();

	static uintptr_t g_client;

private:
	
};
