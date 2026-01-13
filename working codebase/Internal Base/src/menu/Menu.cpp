// Menu.cpp (updated)
#include "Menu.h"
#include "MenuStyles.h"

#include "../../ext/imgui/imgui.h"
#include "../sdk/utils/Globals.h"

#include <Windows.h>

bool Menu::PollKey(int& out)
{
	for (int i = 1; i < 256; i++)
	{
		if (GetAsyncKeyState(i) & 1)
		{
			out = i;
			return true;
		}
	}
	return false;
}

void Menu::createESP()
{
	MenuStyles::CustomCheckbox("Enable ESP", &Globals::esp_enabled);

	ImGui::SameLine(ImGui::GetWindowWidth() - 50.f);
	ImVec2 bind_p = ImGui::GetCursorScreenPos();
	ImDrawList* dl = ImGui::GetWindowDrawList();

	ImGui::InvisibleButton("bind_btn", { 32, 24 });
	bool bind_hovered = ImGui::IsItemHovered();

	if (ImGui::IsItemClicked())
	{
		ESP::bind_popup_pos = ImGui::GetMousePos();
		ESP::wait_for_bind = false;
		ImGui::OpenPopup("ESP Bind");
	}

	if (bind_hovered)
	{
		dl->AddRectFilled({ bind_p.x - 2, bind_p.y - 2 }, { bind_p.x + 34, bind_p.y + 26 }, IM_COL32(130, 90, 255, 40), 5.f);
	}

	ImU32 bind_col = bind_hovered ? IM_COL32(140, 100, 255, 255) : IM_COL32(70, 70, 80, 255);
	dl->AddRectFilled(bind_p, { bind_p.x + 32, bind_p.y + 24 }, bind_col, 4.f);

	dl->AddRectFilledMultiColor(
		bind_p, { bind_p.x + 32, bind_p.y + 12 },
		IM_COL32(255, 255, 255, 20),
		IM_COL32(255, 255, 255, 20),
		IM_COL32(255, 255, 255, 0),
		IM_COL32(255, 255, 255, 0)
	);

	dl->AddRect(bind_p, { bind_p.x + 32, bind_p.y + 24 }, IM_COL32(100, 100, 110, 200), 4.f, 0, 2.f);

	float dot_y = bind_p.y + 12;
	dl->AddCircleFilled({ bind_p.x + 8, dot_y }, 2.5f, IM_COL32(255, 255, 255, 255));
	dl->AddCircleFilled({ bind_p.x + 16, dot_y }, 2.5f, IM_COL32(255, 255, 255, 255));
	dl->AddCircleFilled({ bind_p.x + 24, dot_y }, 2.5f, IM_COL32(255, 255, 255, 255));

	ImGui::SetNextWindowPos(ESP::bind_popup_pos, ImGuiCond_Appearing);
	if (ImGui::BeginPopupModal("ESP Bind", nullptr,
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::TextUnformatted("ESP Toggle Bind");
		ImGui::Separator();
		ImGui::Spacing();

		if (!ESP::wait_for_bind)
		{
			if (ImGui::Button("Set bind", { 140, 0 }))
			{
				ESP::wait_for_bind = true;
			}

			ImGui::SameLine();

			if (ImGui::Button("Close", { 80, 0 }))
			{
				ImGui::CloseCurrentPopup();
			}
		}
		else
		{
			ImGui::TextUnformatted("Press a key...");
			ImGui::TextDisabled("ESC to cancel");

			int key = 0;
			if (PollKey(key))
			{
				if (key != VK_ESCAPE)
				{
					Globals::esp_bind = key;
				}

				ESP::wait_for_bind = false;
				ImGui::CloseCurrentPopup();
			}
		}

		ImGui::EndPopup();
	}
}

void Menu::createSubESP()
{
	ImGui::Indent(25.f);

	MenuStyles::CustomCheckbox("Box", &Globals::esp_box);
	ImGui::SameLine(ImGui::GetWindowWidth() - 150.f);
	MenuStyles::CustomColor("Team color", Globals::esp_box_color);


	ImGui::Spacing();//////////////////////////////////////////

	ImGui::SameLine(ImGui::GetWindowWidth() - 150.f);
	MenuStyles::CustomColor("Enemy color", Globals::esp_box_color_2);


	ImGui::Spacing();//////////////////////////////////////////


	MenuStyles::CustomCheckbox("Skeleton", &Globals::esp_skeleton);
	ImGui::SameLine(ImGui::GetWindowWidth() - 150.f);
	MenuStyles::CustomColor("Skeleton color", Globals::esp_skeleton_color);


	ImGui::Spacing();//////////////////////////////////////////

	MenuStyles::CustomCheckbox("Name", &Globals::esp_name);
	MenuStyles::CustomCheckbox("Health bar", &Globals::esp_health);

	ImGui::Spacing();//////////////////////////////////////////

	MenuStyles::CustomCheckbox("Team ESP", &Globals::esp_teamate);


	ImGui::Unindent(25.f);
}

void Menu::createAimbot()
{
	MenuStyles::CustomCheckbox("Enable Aimbot", &Globals::aimbot_enabled);
	ImGui::Spacing();

	MenuStyles::CustomSlider("FOV", &Globals::aimbot_fov, 0.f, 89.f);
	ImGui::Spacing();

	MenuStyles::CustomCheckbox("Enable Smooth", &Globals::aimbot_smooth);
	ImGui::Spacing();

	MenuStyles::CustomSlider("Smoothness", &Globals::aimbot_smoothness, 0.f, 1.f);
	MenuStyles::CustomCheckbox("Enable Friendly Fire", &Globals::aimbot_friendly_fire);

	MenuStyles::CustomCheckbox("Enable Body Aim", &Globals::aimbot_force_baim);
	MenuStyles::CustomSlider("Body Aim", &Globals::aimbot_baim_min, 0, 100);

	
}

void Menu::createAutoFire()
{
	MenuStyles::CustomCheckbox("Enable Auto Fire", &Globals::autofire_enabled);
}


void Menu::createBhop()
{

	MenuStyles::CustomCheckbox("Enable Bhop", &Globals::bhop_enabled);
}

void Menu::Render()
{
	float dt = ImGui::GetIO().DeltaTime;
	MenuStyles::AdvanceTime(dt);

	MenuStyles::PushWindowStyle();

	ImGui::SetNextWindowSize({ SIZE_X, SIZE_Y }, ImGuiCond_Once);
	ImGui::Begin("TRAF CLIENT", &IsOpen,
	ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

	ImDrawList* dl = ImGui::GetWindowDrawList();
	ImVec2 wp = ImGui::GetWindowPos();
	ImVec2 ws = ImGui::GetWindowSize();
	float win_width = ws.x;

	MenuStyles::InitParticles(wp, ws);
	MenuStyles::UpdateParticles(wp, ws, dt);
	MenuStyles::RenderParticles(dl, wp, ws);

	MenuStyles::RenderBackground(dl, wp, ws);
	MenuStyles::RenderTopAccent(dl, wp, win_width);

	ImGui::Spacing();
	ImGui::Spacing();

	MenuStyles::SectionHeader("ESP", 60.0f);
	createESP();
	ImGui::Spacing();
	createSubESP();

	ImGui::Spacing();
	ImGui::Spacing();

	ImVec2 sep_pos = ImGui::GetCursorScreenPos();
	MenuStyles::RenderSeparator(dl, sep_pos, win_width - 20.0f);

	ImGui::Spacing();
	ImGui::Spacing();

	MenuStyles::SectionHeader("AIMBOT", 85.0f);
	createAimbot();
	createAutoFire();

	MenuStyles::SectionHeader("MISC", 85.0f);
	ImGui::Spacing();
	createBhop();

	ImGui::End();
	MenuStyles::PopWindowStyle();
}
