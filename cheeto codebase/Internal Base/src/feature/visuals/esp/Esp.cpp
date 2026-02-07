#include "Esp.h"
#include "../../../sdk/entity/EntityManager.h"
#include "../../../sdk/entity/Classes.h"
#include "../../../sdk/utils/Utils.h"
#include "../../../sdk/utils/Globals.h"
#include "../../../../ext/imgui/imgui.h"
#include <iostream>
#include <algorithm>

// static members
ImDrawList* ESP::m_dl	 = nullptr;
float		ESP::m_sw	 = 0.0f;
float		ESP::m_sh	 = 0.0f;
ImU32		ESP::m_boxCol1 = 0;
ImU32		ESP::m_boxCol2 = 0;
ImU32		ESP::m_skelCol = 0;
ImU32		ESP::m_nameCol = 0;

void ESP::setup()
{
	// update these every frame
	m_dl = ImGui::GetBackgroundDrawList();
	m_sw = ImGui::GetIO().DisplaySize.x;
	m_sh = ImGui::GetIO().DisplaySize.y;
	
	m_boxCol1 = ImGui::ColorConvertFloat4ToU32(ImVec4(
	Globals::esp_box_color[0], Globals::esp_box_color[1],
	Globals::esp_box_color[2], Globals::esp_box_color[3]));

	m_boxCol2 = ImGui::ColorConvertFloat4ToU32(ImVec4(
	Globals::esp_box_color_2[0], Globals::esp_box_color_2[1],
	Globals::esp_box_color_2[2], Globals::esp_box_color_2[3]));

	m_skelCol = ImGui::ColorConvertFloat4ToU32(ImVec4(
	Globals::esp_skeleton_color[0], Globals::esp_skeleton_color[1],
	Globals::esp_skeleton_color[2], Globals::esp_skeleton_color[3]));

	m_nameCol = ImGui::ColorConvertFloat4ToU32(ImVec4(
	Globals::esp_name_color[0], Globals::esp_name_color[1],
	Globals::esp_name_color[2], Globals::esp_name_color[3]));


}

void ESP::renderEntity(Entity_t& ent, C_CSPlayerPawn* localPawn)
{
	C_CSPlayerPawn* pawn = ent.pawn;
	if (!Globals::esp_enabled) return;
	if (!localPawn || !pawn->IsAlive()) return;
	

	const ImU32 boxCol = (pawn->m_iTeamNum() == localPawn->m_iTeamNum()) ? m_boxCol1 : m_boxCol2;

	if (pawn->m_iTeamNum() == localPawn->m_iTeamNum() && !Globals::esp_teamate) return;
	
	Vector feet = pawn->m_vOldOrigin();
	Vector head = Utils::GetBonePos(pawn, BoneID::Head);

	if (head.IsZero())
	{
		return;
	}

	head.z += 8.2f;

	Vector sFeet, sHead;
	if (!Utils::WorldToScreen(feet, sFeet, (float*)Globals::ViewMatrix, m_sw, m_sh) ||
	!Utils::WorldToScreen(head, sHead, (float*)Globals::ViewMatrix, m_sw, m_sh)) return;

	float h = sFeet.y - sHead.y;
	if (h < 5.f) return;

	float w = h * 0.5f;
	float x = sHead.x - w * 0.5f;
	float y = sHead.y;

	if (Globals::esp_box)
	{
		m_dl->AddRect({ x, y }, { x + w, y + h }, boxCol, 0.f, 0, Globals::esp_box_thickness);
		m_dl->AddRect({ x - 1, y - 1 }, { x + w + 1, y + h + 1 }, IM_COL32(0, 0, 0, 220));
	}

	if (Globals::esp_health)
	{
		int	  hp = pawn->m_iHealth();
		float hpFrac = std::clamp(hp / 100.f, 0.f, 1.f);
		float hpH = h * hpFrac;

		m_dl->AddRectFilled({ x - 6, y - 1 }, { x - 2, y + h + 1 }, IM_COL32(0, 0, 0, 150));
		m_dl->AddRectFilled({ x - 5, y + h - hpH }, { x - 3, y + h }, IM_COL32(0, 255, 0, 255));
	}

	// TODO FIX (FEBRUARY) need to fix names
	//if (Globals::esp_name && ent.controller)
	//{
	//	char nameBuf[128]{};
	//	uintptr_t namePtr = reinterpret_cast<uintptr_t>(ent.controller->m_szTeamname());
	//	if (Utils::SafeReadString(namePtr, nameBuf))
	//	{
	//		ImVec2 ts = ImGui::CalcTextSize(nameBuf);
	//		m_dl->AddText({ x + (w - ts.x) * 0.5f, y - ts.y - 2 }, m_nameCol, nameBuf);
	//	}
	//}

	if (Globals::esp_skeleton)
	{
		const float thick = Globals::esp_skeleton_thickness;

		for (const auto& conn : Bones::connections)
		{
			Vector b1 = Utils::GetBonePos(pawn, conn.bone1);
			Vector b2 = Utils::GetBonePos(pawn, conn.bone2);

			if (b1.IsZero() || b2.IsZero()) continue;

			Vector sb1, sb2;
			if (Utils::WorldToScreen(b1, sb1, (float*)Globals::ViewMatrix, m_sw, m_sh) &&
			Utils::WorldToScreen(b2, sb2, (float*)Globals::ViewMatrix, m_sw, m_sh))
			{
				m_dl->AddLine({ sb1.x, sb1.y }, { sb2.x, sb2.y }, m_skelCol, thick);
			}
		}
	}
}

