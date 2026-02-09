#pragma once

#include "MenuStyles.h"
#include <type_traits>
#include <cmath>
#include <algorithm>
#include <vector>
#include <string>

namespace MenuStyles
{
	template <typename T>
	void CustomSlider(const char* label, T* v, T minVal, T maxVal)
	{
		static_assert(std::is_arithmetic_v<T>, "CustomSlider requires an arithmetic type.");

		ImGui::PushID(label);
		ImVec2 p = ImGui::GetCursorScreenPos();
		ImDrawList* dl = ImGui::GetWindowDrawList();

		const float slider_width = 200.f;
		const float slider_height = 24.f;
		const float grab_size = 10.f;

		ImGui::InvisibleButton("slider", { slider_width, slider_height });

		const bool is_active = ImGui::IsItemActive();
		const bool is_hovered = ImGui::IsItemHovered();

		if (maxVal != minVal)
		{
			if (is_active && ImGui::IsMouseDragging(0, 0.f))
			{
				float mouse_x = ImGui::GetMousePos().x - p.x;
				float t = std::clamp(mouse_x / slider_width, 0.0f, 1.0f);

				// Interpolate in float/double space
				using F = std::conditional_t<std::is_same_v<T, long double>, long double, double>;
				F newValue = static_cast<F>(minVal) + static_cast<F>(t) * (static_cast<F>(maxVal) - static_cast<F>(minVal));

				if constexpr (std::is_integral_v<T>)
				{
					// snap to integer
					newValue = std::round(newValue);
				}

				// clamp back to bounds and assign
				if (newValue < static_cast<F>(minVal))
					newValue = static_cast<F>(minVal);
				if (newValue > static_cast<F>(maxVal))
					newValue = static_cast<F>(maxVal);

				*v = static_cast<T>(newValue);
			}
		}

		float percentage = 0.0f;
		if (maxVal != minVal)
		{
			percentage = (static_cast<float>(*v) - static_cast<float>(minVal)) /
			(static_cast<float>(maxVal) - static_cast<float>(minVal));
			percentage = std::clamp(percentage, 0.0f, 1.0f);
		}

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

		if constexpr (std::is_integral_v<T>)
		{
			ImGui::Text("%s: %d", label, static_cast<int>(*v));
		}
		else
		{
			ImGui::Text("%s: %.1f", label, static_cast<float>(*v));
		}

		ImGui::PopID();
	}


	template <typename T>
	void CustomSelection(const char* label, int* current_index, const std::vector<T>& items)
	{
		ImGui::PushID(label);

		ImVec2		p	   = ImGui::GetCursorScreenPos();
		ImDrawList* dl	   = ImGui::GetWindowDrawList();
		float		width  = 180.f;
		float		height = 24.f;

		ImGui::InvisibleButton("selection", { width, height });
		bool hovered = ImGui::IsItemHovered();

		if (ImGui::IsItemClicked())
			ImGui::OpenPopup("options");

		if (hovered)
			dl->AddRectFilled(
			{ p.x - 2, p.y - 2 },
			{ p.x + width + 2, p.y + height + 2 },
			MenuColors::purple_glow,
			6.f);

		ImU32 bg_col = hovered ? MenuColors::hover_gray : MenuColors::dark_gray;
		dl->AddRectFilled(p, { p.x + width, p.y + height }, bg_col, 5.f);

		ImU32 border_col = hovered ? MenuColors::light_purple : MenuColors::gray;
		dl->AddRect(p, { p.x + width, p.y + height }, border_col, 5.f, 0, 2.f);

		if (*current_index >= 0 && *current_index < (int)items.size())
		{
			ImVec2 text_pos = { p.x + 8, p.y + 4 };

			if constexpr (std::is_same_v<T, const char*>)
			{
				dl->AddText(text_pos, MenuColors::white, items[*current_index]);
			}
			else if constexpr (std::is_same_v<T, std::string>)
			{
				dl->AddText(text_pos, MenuColors::white, items[*current_index].c_str());
			}
			else
			{
				dl->AddText(text_pos, MenuColors::white, std::to_string(items[*current_index]).c_str());
			}
		}

		ImVec2 arrow_center	   = { p.x + width - 15, p.y + height / 2 };
		float  arrow_size	   = 4.f;
		ImVec2 arrow_points[3] = {
			{ arrow_center.x - arrow_size, arrow_center.y - arrow_size / 2 },
			{ arrow_center.x + arrow_size, arrow_center.y - arrow_size / 2 },
			{ arrow_center.x, arrow_center.y + arrow_size }
		};
		dl->AddTriangleFilled(arrow_points[0], arrow_points[1], arrow_points[2], MenuColors::light_gray);

		if (ImGui::BeginPopup("options"))
		{
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(5, 5));

			for (size_t i = 0; i < items.size(); i++)
			{
				ImVec2 option_p = ImGui::GetCursorScreenPos();

				ImGui::PushID((int)i);

				const char* item_text;
				std::string temp_str;

				if constexpr (std::is_same_v<T, const char*>)
				{
					item_text = items[i];
				}
				else if constexpr (std::is_same_v<T, std::string>)
				{
					item_text = items[i].c_str();
				}
				else
				{
					temp_str  = std::to_string(items[i]);
					item_text = temp_str.c_str();
				}

				if (ImGui::Selectable(item_text, *current_index == (int)i, 0, { width - 10, 20 }))
					*current_index = (int)i;

				ImGui::PopID();

				if (*current_index == (int)i)
				{
					ImDrawList* popup_dl = ImGui::GetWindowDrawList();
					popup_dl->AddRectFilled(
					{ option_p.x - 3, option_p.y },
					{ option_p.x, option_p.y + 20 },
					MenuColors::vibrant_purple,
					2.f);
				}
			}

			ImGui::PopStyleVar();
			ImGui::EndPopup();
		}

		ImGui::SameLine();
		ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 4);
		ImGui::TextUnformatted(label);

		ImGui::PopID();
	}

}
