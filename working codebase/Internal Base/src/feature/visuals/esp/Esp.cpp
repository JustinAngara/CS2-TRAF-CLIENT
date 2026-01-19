#include "Esp.h"
#include "../../../sdk/entity/EntityManager.h"
#include "../../../sdk/entity/Classes.h"
#include "../../../sdk/utils/Utils.h"
#include "../../../sdk/utils/Globals.h"
#include "../../../../ext/imgui/imgui.h"
#include <algorithm>


void ESP::render()
{
	// Your render loop implementation
}

void ESP::renderEntity(C_CSPlayerPawn* pawn, C_CSPlayerPawn* localPawn)
{
	if (!Globals::esp_enabled) return;

	// Static variables - initialized once, persist between calls
	static ImDrawList* dl	   = nullptr;
	static float	   sw	   = 0.0f;
	static float	   sh	   = 0.0f;
	static ImU32	   boxCol1 = 0;
	static ImU32	   boxCol2 = 0;
	static ImU32	   skelCol = 0;
	static ImU32	   nameCol = 0;

	// Update these every frame
	dl = ImGui::GetBackgroundDrawList();
	sw = ImGui::GetIO().DisplaySize.x;
	sh = ImGui::GetIO().DisplaySize.y;

	boxCol1 = ImGui::ColorConvertFloat4ToU32(ImVec4(
	Globals::esp_box_color[0], Globals::esp_box_color[1],
	Globals::esp_box_color[2], Globals::esp_box_color[3]));

	boxCol2 = ImGui::ColorConvertFloat4ToU32(ImVec4(
	Globals::esp_box_color_2[0], Globals::esp_box_color_2[1],
	Globals::esp_box_color_2[2], Globals::esp_box_color_2[3]));

	skelCol = ImGui::ColorConvertFloat4ToU32(ImVec4(
	Globals::esp_skeleton_color[0], Globals::esp_skeleton_color[1],
	Globals::esp_skeleton_color[2], Globals::esp_skeleton_color[3]));

	nameCol = ImGui::ColorConvertFloat4ToU32(ImVec4(
	Globals::esp_name_color[0], Globals::esp_name_color[1],
	Globals::esp_name_color[2], Globals::esp_name_color[3]));

	if (!pawn || !pawn->IsAlive()) return;

	// Get controller from the pawn instead of EntityManager
	C_CSPlayerController* controller = EntityManager::Get().GetLocalPlayer(); 

	const ImU32 boxCol = (pawn->m_iTeamNum() == localPawn->m_iTeamNum()) ? boxCol1 : boxCol2;

	if (pawn->m_iTeamNum() == localPawn->m_iTeamNum() && !Globals::esp_teamate) return;

	Vector feet = pawn->m_vOldOrigin();
	Vector head = Utils::GetBonePos(pawn, BoneID::Head);
	if (head.IsZero()) return;

	head.z += 8.2f;

	Vector sFeet, sHead;
	if (!Utils::WorldToScreen(feet, sFeet, (float*)Globals::ViewMatrix, sw, sh) ||
	!Utils::WorldToScreen(head, sHead, (float*)Globals::ViewMatrix, sw, sh)) return;

	float h = sFeet.y - sHead.y;
	if (h < 5.f) return;

	float w = h * 0.5f;
	float x = sHead.x - w * 0.5f;
	float y = sHead.y;

	if (Globals::esp_box)
	{
		dl->AddRect({ x, y }, { x + w, y + h }, boxCol, 0.f, 0, Globals::esp_box_thickness);
		dl->AddRect({ x - 1, y - 1 }, { x + w + 1, y + h + 1 }, IM_COL32(0, 0, 0, 220));
	}

	if (Globals::esp_health)
	{
		int	  hp	 = pawn->m_iHealth();
		float hpFrac = std::clamp(hp / 100.f, 0.f, 1.f);
		float hpH	 = h * hpFrac;

		dl->AddRectFilled({ x - 6, y - 1 }, { x - 2, y + h + 1 }, IM_COL32(0, 0, 0, 150));
		dl->AddRectFilled({ x - 5, y + h - hpH }, { x - 3, y + h }, IM_COL32(0, 255, 0, 255));
	}

	if (Globals::esp_name && controller)
	{
		char	  nameBuf[128]{};
		uintptr_t namePtr = reinterpret_cast<uintptr_t>(controller->m_szTeamname());
		if (Utils::SafeReadString(namePtr, nameBuf))
		{
			ImVec2 ts = ImGui::CalcTextSize(nameBuf);
			dl->AddText({ x + (w - ts.x) * 0.5f, y - ts.y - 2 }, nameCol, nameBuf);
		}
	}

	if (Globals::esp_skeleton)
	{
		const float thick = Globals::esp_skeleton_thickness;

		for (const auto& conn : Bones::connections)
		{
			Vector b1 = Utils::GetBonePos(pawn, conn.bone1);
			Vector b2 = Utils::GetBonePos(pawn, conn.bone2);

			if (b1.IsZero() || b2.IsZero()) continue;

			Vector sb1, sb2;
			if (Utils::WorldToScreen(b1, sb1, (float*)Globals::ViewMatrix, sw, sh) &&
			Utils::WorldToScreen(b2, sb2, (float*)Globals::ViewMatrix, sw, sh))
			{
				dl->AddLine({ sb1.x, sb1.y }, { sb2.x, sb2.y }, skelCol, thick);
			}
		}
	}
}

