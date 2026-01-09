#include "MenuStyles.h"

#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <vector>

struct Particle
{
	ImVec2 pos;
	ImVec2 vel;
	float size;
	float alpha;
	float life;
};

static std::vector<Particle> g_particles;
static float g_time = 0.0f;
static bool g_particles_initialized = false;

namespace MenuStyles
{
	void AdvanceTime(float dt)
	{
		g_time += dt;
	}

	float Time()
	{
		return g_time;
	}

	void PushWindowStyle()
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 12.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(15, 15));
	}

	void PopWindowStyle()
	{
		ImGui::PopStyleVar(3);
	}

	void InitParticles(ImVec2 window_pos, ImVec2 window_size, int count)
	{
		if (g_particles_initialized)
			return;

		g_particles.clear();
		g_particles.reserve(count);

		for (int i = 0; i < count; i++)
		{
			Particle p;
			p.pos.x = window_pos.x + (std::rand() % (int)window_size.x);
			p.pos.y = window_pos.y + (std::rand() % (int)window_size.y);
			p.vel.x = ((std::rand() % 100 - 50) / 100.0f) * 0.5f;
			p.vel.y = ((std::rand() % 100 - 50) / 100.0f) * 0.5f;
			p.size = 1.5f + (std::rand() % 30) / 10.0f;
			p.alpha = 0.3f + (std::rand() % 70) / 100.0f;
			p.life = 1.0f;
			g_particles.push_back(p);
		}

		g_particles_initialized = true;
	}

	void UpdateParticles(ImVec2 window_pos, ImVec2 window_size, float dt)
	{
		for (auto& p : g_particles)
		{
			p.pos.x += p.vel.x * 30.0f * dt;
			p.pos.y += p.vel.y * 30.0f * dt;

			if (p.pos.x < window_pos.x)
				p.pos.x = window_pos.x + window_size.x;
			if (p.pos.x > window_pos.x + window_size.x)
				p.pos.x = window_pos.x;
			if (p.pos.y < window_pos.y)
				p.pos.y = window_pos.y + window_size.y;
			if (p.pos.y > window_pos.y + window_size.y)
				p.pos.y = window_pos.y;
		}
	}

	void RenderParticles(ImDrawList* dl, ImVec2 window_pos, ImVec2 window_size)
	{
		(void)window_pos;
		(void)window_size;

		for (size_t i = 0; i < g_particles.size(); i++)
		{
			for (size_t j = i + 1; j < g_particles.size(); j++)
			{
				float dx = g_particles[i].pos.x - g_particles[j].pos.x;
				float dy = g_particles[i].pos.y - g_particles[j].pos.y;
				float dist = std::sqrt(dx * dx + dy * dy);

				if (dist < 120.0f)
				{
					float a = (1.0f - dist / 120.0f) * 0.3f;
					ImU32 line_col = IM_COL32(130, 90, 255, (int)(a * 255.0f));
					dl->AddLine(g_particles[i].pos, g_particles[j].pos, line_col, 1.0f);
				}
			}
		}

		for (const auto& p : g_particles)
		{
			ImU32 glow_col = IM_COL32(255, 255, 255, (int)(p.alpha * 40.0f));
			ImU32 particle_col = IM_COL32(255, 255, 255, (int)(p.alpha * 200.0f));

			dl->AddCircleFilled(p.pos, p.size * 2.0f, glow_col, 16);
			dl->AddCircleFilled(p.pos, p.size, particle_col, 12);

			ImVec2 highlight_pos = { p.pos.x - p.size * 0.3f, p.pos.y - p.size * 0.3f };
			dl->AddCircleFilled(
			highlight_pos,
			p.size * 0.5f,
			IM_COL32(255, 255, 255, (int)(p.alpha * 255.0f)),
			8);
		}
	}

	void RenderBackground(ImDrawList* dl, ImVec2 wp, ImVec2 ws)
	{
		float win_width = ws.x;
		float win_height = ws.y;

		dl->AddRectFilled(
		wp,
		{ wp.x + win_width, wp.y + win_height },
		IM_COL32(8, 8, 12, 160),
		12.0f);

		dl->AddRectFilled(
		{ wp.x + 1, wp.y + 1 },
		{ wp.x + win_width - 1, wp.y + win_height - 1 },
		IM_COL32(12, 12, 18, 120),
		12.0f);

		dl->AddRectFilled(
		{ wp.x + 2, wp.y + 2 },
		{ wp.x + win_width - 2, wp.y + win_height - 2 },
		IM_COL32(18, 18, 25, 90),
		12.0f);

		dl->AddRectFilledMultiColor(
		wp,
		{ wp.x + win_width, wp.y + 80 },
		IM_COL32(0, 0, 0, 100),
		IM_COL32(0, 0, 0, 100),
		IM_COL32(0, 0, 0, 0),
		IM_COL32(0, 0, 0, 0));

		dl->AddRectFilledMultiColor(
		{ wp.x, wp.y + win_height - 80 },
		{ wp.x + win_width, wp.y + win_height },
		IM_COL32(0, 0, 0, 0),
		IM_COL32(0, 0, 0, 0),
		IM_COL32(0, 0, 0, 100),
		IM_COL32(0, 0, 0, 100));

		dl->AddRectFilledMultiColor(
		wp,
		{ wp.x + 60, wp.y + win_height },
		IM_COL32(0, 0, 0, 80),
		IM_COL32(0, 0, 0, 0),
		IM_COL32(0, 0, 0, 0),
		IM_COL32(0, 0, 0, 80));

		dl->AddRectFilledMultiColor(
		{ wp.x + win_width - 60, wp.y },
		{ wp.x + win_width, wp.y + win_height },
		IM_COL32(0, 0, 0, 0),
		IM_COL32(0, 0, 0, 80),
		IM_COL32(0, 0, 0, 80),
		IM_COL32(0, 0, 0, 0));
	}

	void RenderTopAccent(ImDrawList* dl, ImVec2 wp, float win_width)
	{
		float pulse = (std::sin(g_time * 2.0f) * 0.5f + 0.5f) * 30.0f;
		int p = (int)pulse;

		dl->AddRectFilledMultiColor(
		wp,
		{ wp.x + win_width, wp.y + 4 },
		IM_COL32(90 + p, 60, 180, 255),
		IM_COL32(140 + p, 100, 255, 255),
		IM_COL32(140 + p, 100, 255, 255),
		IM_COL32(90 + p, 60, 180, 255));

		float glow_intensity = (std::sin(g_time * 3.0f) * 0.5f + 0.5f);
		int a = (int)(60.0f * glow_intensity);

		dl->AddRectFilledMultiColor(
		{ wp.x, wp.y + 4 },
		{ wp.x + win_width, wp.y + 12 },
		IM_COL32(130, 90, 255, a),
		IM_COL32(130, 90, 255, a),
		IM_COL32(130, 90, 255, 0),
		IM_COL32(130, 90, 255, 0));
	}

	void RenderSeparator(ImDrawList* dl, ImVec2 sep_pos, float width)
	{
		float sep_flow = (std::sin(g_time * 1.5f) * 0.5f + 0.5f);
		int a = (int)(120.0f * sep_flow);

		dl->AddRectFilledMultiColor(
		{ sep_pos.x - 10, sep_pos.y },
		{ sep_pos.x + width, sep_pos.y + 2 },
		IM_COL32(130, 90, 255, 0),
		IM_COL32(130, 90, 255, a),
		IM_COL32(130, 90, 255, a),
		IM_COL32(130, 90, 255, 0));
	}

	void SectionHeader(const char* title, float glow_width)
	{
		ImDrawList* dl = ImGui::GetWindowDrawList();
		ImVec2 header_pos = ImGui::GetCursorScreenPos();

		float header_glow = (std::sin(g_time * 2.0f) * 0.3f + 0.7f);
		int a = (int)(40.0f * header_glow);

		dl->AddRectFilled(
		{ header_pos.x - 5, header_pos.y - 2 },
		{ header_pos.x + glow_width, header_pos.y + 24 },
		IM_COL32(130, 90, 255, a),
		5.f);

		ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(150, 110, 255, 255));
		ImGui::TextUnformatted(title);
		ImGui::PopStyleColor();
		ImGui::Spacing();
	}

	void CustomCheckbox(const char* label, bool* v)
	{
		ImGui::PushID(label);

		ImVec2 p = ImGui::GetCursorScreenPos();
		ImDrawList* dl = ImGui::GetWindowDrawList();
		float sz = 20.f;

		ImGui::InvisibleButton("checkbox", { sz, sz });
		bool hovered = ImGui::IsItemHovered();
		if (ImGui::IsItemClicked())
			*v = !*v;

		ImU32 glow_col = hovered ? IM_COL32(130, 90, 255, 60) : IM_COL32(0, 0, 0, 0);
		if (hovered || *v)
			dl->AddRectFilled({ p.x - 2, p.y - 2 }, { p.x + sz + 2, p.y + sz + 2 }, glow_col, 6.f);

		if (*v)
		{
			dl->AddRectFilledMultiColor(
			p, { p.x + sz, p.y + sz },
			IM_COL32(120, 80, 230, 255),
			IM_COL32(150, 100, 255, 255),
			IM_COL32(150, 100, 255, 255),
			IM_COL32(120, 80, 230, 255));
		}
		else
		{
			ImU32 bg_col = hovered ? IM_COL32(65, 65, 75, 255) : IM_COL32(45, 45, 55, 255);
			dl->AddRectFilled(p, { p.x + sz, p.y + sz }, bg_col, 5.f);
		}

		ImU32 border_col = *v ? IM_COL32(160, 120, 255, 255) : IM_COL32(80, 80, 90, 255);
		dl->AddRect(p, { p.x + sz, p.y + sz }, border_col, 5.f, 0, 2.f);

		if (*v)
		{
			ImVec2 check_start = { p.x + 5, p.y + sz / 2 };
			ImVec2 check_mid = { p.x + sz / 2 - 1, p.y + sz - 6 };
			ImVec2 check_end = { p.x + sz - 4, p.y + 4 };

			dl->AddLine(check_start, check_mid, IM_COL32(255, 255, 255, 255), 2.5f);
			dl->AddLine(check_mid, check_end, IM_COL32(255, 255, 255, 255), 2.5f);
		}

		ImGui::SameLine();
		ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 3);
		ImGui::TextUnformatted(label);

		ImGui::PopID();
	}

	void CustomColor(const char* label, float col[4])
	{
		ImGui::PushID(label);

		ImVec2 p = ImGui::GetCursorScreenPos();
		ImDrawList* dl = ImGui::GetWindowDrawList();

		ImU32 c = ImGui::ColorConvertFloat4ToU32(ImVec4(col[0], col[1], col[2], col[3]));

		ImGui::InvisibleButton("color", { 28, 20 });
		bool hovered = ImGui::IsItemHovered();

		if (ImGui::IsItemClicked())
			ImGui::OpenPopup("picker");

		if (hovered)
			dl->AddRectFilled({ p.x - 2, p.y - 2 }, { p.x + 30, p.y + 22 }, IM_COL32(130, 90, 255, 40), 5.f);

		dl->AddRectFilled(p, { p.x + 28, p.y + 20 }, c, 4.f);

		dl->AddRectFilledMultiColor(
		{ p.x + 2, p.y + 2 }, { p.x + 26, p.y + 8 },
		IM_COL32(255, 255, 255, 30),
		IM_COL32(255, 255, 255, 30),
		IM_COL32(255, 255, 255, 0),
		IM_COL32(255, 255, 255, 0));

		ImU32 border_col = hovered ? IM_COL32(150, 110, 255, 255) : IM_COL32(90, 90, 100, 255);
		dl->AddRect(p, { p.x + 28, p.y + 20 }, border_col, 4.f, 0, 2.f);

		if (ImGui::BeginPopup("picker"))
		{
			ImGui::ColorPicker4(
			"picker", col,
			ImGuiColorEditFlags_NoInputs |
			ImGuiColorEditFlags_NoSidePreview |
			ImGuiColorEditFlags_NoSmallPreview);
			ImGui::EndPopup();
		}

		ImGui::SameLine();
		ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 2);
		ImGui::TextUnformatted(label);

		ImGui::PopID();
	}

	void CustomSlider(const char* label, float* v, float minVal, float maxVal)
	{
		ImGui::PushID(label);
		ImVec2 p = ImGui::GetCursorScreenPos();
		ImDrawList* dl = ImGui::GetWindowDrawList();

		float slider_width = 200.f;
		float slider_height = 24.f;
		float grab_size = 10.f;

		ImGui::InvisibleButton("slider", { slider_width, slider_height });

		bool is_active = ImGui::IsItemActive();
		bool is_hovered = ImGui::IsItemHovered();

		if (is_active && ImGui::IsMouseDragging(0, 0.f))
		{
			float mouse_x = ImGui::GetMousePos().x - p.x;
			float percentage = std::clamp(mouse_x / slider_width, 0.0f, 1.0f);
			*v = minVal + percentage * (maxVal - minVal);
		}

		float percentage = (*v - minVal) / (maxVal - minVal);
		percentage = std::clamp(percentage, 0.0f, 1.0f);
		float grab_x = p.x + percentage * slider_width;

		if (is_hovered || is_active)
			dl->AddRectFilled({ p.x - 2, p.y - 2 }, { p.x + slider_width + 2, p.y + slider_height + 2 }, IM_COL32(130, 90, 255, 30), 7.f);

		dl->AddRectFilled({ p.x, p.y + 2 }, { p.x + slider_width, p.y + slider_height - 2 }, IM_COL32(30, 30, 40, 255), 6.f);
		dl->AddRectFilled({ p.x + 1, p.y + 3 }, { p.x + slider_width - 1, p.y + 6 }, IM_COL32(0, 0, 0, 80), 5.f);

		if (percentage > 0.01f)
		{
			dl->AddRectFilledMultiColor(
			{ p.x, p.y + 2 }, { grab_x, p.y + slider_height - 2 },
			IM_COL32(90, 60, 180, 255),
			IM_COL32(140, 100, 255, 255),
			IM_COL32(140, 100, 255, 255),
			IM_COL32(90, 60, 180, 255));

			dl->AddRectFilledMultiColor(
			{ p.x + 2, p.y + 4 }, { grab_x - 2, p.y + 10 },
			IM_COL32(255, 255, 255, 40),
			IM_COL32(255, 255, 255, 40),
			IM_COL32(255, 255, 255, 0),
			IM_COL32(255, 255, 255, 0));
		}

		dl->AddRect({ p.x, p.y + 2 }, { p.x + slider_width, p.y + slider_height - 2 }, IM_COL32(70, 70, 80, 255), 6.f, 0, 1.5f);

		float grab_y = p.y + slider_height / 2;

		dl->AddCircleFilled({ grab_x + 1, grab_y + 1 }, grab_size + 2, IM_COL32(0, 0, 0, 100), 20);

		if (is_active)
			dl->AddCircleFilled({ grab_x, grab_y }, grab_size + 4, IM_COL32(160, 120, 255, 60), 20);

		ImU32 grab_col1 = is_active ? IM_COL32(180, 140, 255, 255) : IM_COL32(240, 240, 250, 255);
		dl->AddCircleFilled({ grab_x, grab_y }, grab_size, grab_col1, 20);
		dl->AddCircleFilled({ grab_x, grab_y - 3 }, grab_size - 3, IM_COL32(255, 255, 255, 60), 20);

		dl->AddCircle({ grab_x, grab_y }, grab_size, IM_COL32(100, 100, 110, 200), 20, 2.f);

		ImGui::SameLine();
		ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5);
		ImGui::Text("%s: %.1f", label, *v);

		ImGui::PopID();
	}
}
