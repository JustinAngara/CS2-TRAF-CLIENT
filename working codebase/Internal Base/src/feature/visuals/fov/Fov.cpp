#include "Fov.h"
#include "../../../../ext/imgui/imgui.h"
#include "../../../sdk/utils/Globals.h"
#include "../../../sdk/utils/Utils.h"
#include <cmath>



void FOV::Render()
{
	ImVec2 screenSize = ImGui::GetIO().DisplaySize;
	ImVec2 center	  = ImVec2(screenSize.x * 0.5f, screenSize.y * 0.5f);

	float aimbotFov = Globals::aimbot_fov;
	if (aimbotFov <= 0.0f) return;

	// used for 1920x1080 screens
	float realFov = 106.26f;

	float radius = tanf(aimbotFov * (float)M_PI / 360.0f) / tanf(realFov * (float)M_PI / 360.0f) * screenSize.x;

	if (radius <= 0.0f) return;

	ImDrawList* drawList = ImGui::GetBackgroundDrawList();

	drawList->AddCircle(
	center,
	radius,
	IM_COL32(255, 255, 255, 255),
	100,
	1.0f);
}
