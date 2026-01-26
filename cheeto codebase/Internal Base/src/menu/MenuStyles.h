
#pragma once

#include "../../ext/imgui/imgui.h"

struct ImDrawList;
namespace MenuStyles
{


	// time
	void AdvanceTime(float dt);
	float Time();

	// window style stack
	void PushWindowStyle();
	void PopWindowStyle();

	// particles
	void InitParticles(ImVec2 window_pos, ImVec2 window_size, int count = 80);
	void UpdateParticles(ImVec2 window_pos, ImVec2 window_size, float dt);
	void RenderParticles(ImDrawList* dl, ImVec2 window_pos, ImVec2 window_size);

	// background 
	void RenderBackground(ImDrawList* dl, ImVec2 window_pos, ImVec2 window_size);
	void RenderTopAccent(ImDrawList* dl, ImVec2 window_pos, float win_width);
	void RenderSeparator(ImDrawList* dl, ImVec2 sep_pos, float width);
	void SectionHeader(const char* title, float glow_width);

	// custom controls
	void CustomCheckbox(const char* label, bool* v);
	void CustomColor(const char* label, float col[4]);

	template<typename T>
	void CustomSlider(const char* label, T* v, T minVal, T maxVal);
}


#include "MenuStyles.inl"
