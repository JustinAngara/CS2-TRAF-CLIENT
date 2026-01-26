#pragma once
#include <cstdint>

// TODO: create a bunch of getter methods
// use a std::pair style of status and return like getViewAngles, client, etc and other shit like that
class HackManager
{
public:
	// main entry point
	static void Loop();

	// getter methods 
	static void RunFeatures();

	static uintptr_t g_client;

private:
	// i can internally call many shits here and just call it setup instead

	
};
