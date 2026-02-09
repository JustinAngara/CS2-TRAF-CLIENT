#pragma once
#include "../../ext/imgui/imgui.h"

namespace Menu
{

	// vars
	namespace ESP
	{
		static bool wait_for_bind = false; // specefically for esp
		static ImVec2 bind_popup_pos = {}; // esp
	}
	static constexpr int SIZE_X = 640, SIZE_Y = 520;
    inline bool IsOpen = false;


    void		Render();
	static bool PollKey(int& out);
	static void createSubESP();
	static void createESP();
	static void createAimbot();
	static void createBonesSelection();
	static void createBhop();
	static void createAutoFire();
	static void createNoRecoil();
	static void createFovCrosshair();
	static void createAutoStop();


	

}
