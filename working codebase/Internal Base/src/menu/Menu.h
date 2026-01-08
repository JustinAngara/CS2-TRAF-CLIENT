#pragma once
#include "../../ext/imgui/imgui.h"

namespace Menu {

	// vars
	static bool wait_for_bind = false;
	static ImVec2 bind_popup_pos = {};
	static constexpr int SIZE_X = 320, SIZE_Y = 260;
    inline bool IsOpen = false;


    void Render();
	static void CustomSlider(const char* label, float* v, float minVal, float maxVal);
	static void CustomColor(const char* label, float col[4]);
	static void CustomCheckbox(const char* label, bool* v);
	static bool PollKey(int& out);
	static void createSubESP();
	static void createESP();
	static void createAimbot();

	

}
