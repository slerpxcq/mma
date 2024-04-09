#include "mmpch.hpp"
#include "Sequencer.hpp"

#include "Core/Utility/Type.hpp"

#include "Core/App/Application.hpp"
#include "Core/App/Layer/ImGuiLayer.hpp"

#include <imgui_internal.h>

namespace mm
{
	static void ExpandButton(const ImVec2& pos, bool& expanded)
	{
		static constexpr ImVec2 BUTTON_SIZE = ImVec2(10, 10);
		static constexpr uint32_t BUTTON_COLOR = 0x7fc0c0c0;
		ImGuiIO& io = ImGui::GetIO();
		ImDrawList* drawList = ImGui::GetWindowDrawList();

		ImRect rect = ImRect(pos, pos + BUTTON_SIZE);
		bool over = rect.Contains(io.MousePos);
		bool down = over && rect.Contains(io.MouseClickedPos[0]);
		bool clicked = down && io.MouseReleased[0];

		if (clicked) {
			expanded = !expanded;
		}

		ImVec2 mid = (rect.Min + rect.Max) * 0.5f;
		if (!expanded) {
			drawList->AddTriangleFilled(
				rect.Min,
				mid,
				ImVec2(rect.Max.x, rect.Min.y),
				BUTTON_COLOR);
		}
		else {
			drawList->AddTriangleFilled(
				ImVec2(rect.Min.x, mid.y),
				ImVec2(mid.x, rect.Min.y),
				ImVec2(rect.Max.x, mid.y),
				BUTTON_COLOR);
		}
	}

	void Sequencer::OnUIRender()
	{
		static constexpr uint32_t ITEM_HEIGHT = 20;
		static constexpr uint32_t BACKGROUND_COLOR = 0xff242424;
		static constexpr uint32_t HEADER_COLOR = 0xff3d3837;
		static constexpr uint32_t LEGEND_LENGTH = 100;
		static constexpr uint32_t RULER_SHORT_MARK_STEP = 20;
		static constexpr uint32_t RULER_LONG_MARK_MULTIPLIER = 5;
		static constexpr uint32_t RULER_LONG_MARK_LENGTH = 10;
		static constexpr uint32_t RULER_SHORT_MARK_LENGTH = 5;
		static constexpr uint32_t RULER_MARK_COLOR = 0xffc0c0c0;
		static constexpr uint32_t VERTICAL_MARK_COLOR = 0x20c0c0c0;
		static constexpr uint32_t RULER_SCALE = 20;
		static constexpr uint32_t RULER_TEXT_OFFSET = 5;
		static constexpr uint32_t LEGEND_TEXT_OFFSET = 5;
		static constexpr uint32_t STRIP_COLOR_EVEN = 0xff363636;
		static constexpr uint32_t STRIP_COLOR_ODD = 0xff413d3d;

		ImGui::Begin("Sequencer");
		ImDrawList* drawList = ImGui::GetWindowDrawList();
		ImVec2 min = ImGui::GetWindowContentRegionMin();
		ImVec2 max = ImGui::GetWindowContentRegionMax();
		ImVec2 origin = ImGui::GetWindowPos() + min;
		ImVec2 size = max - min;
		ImGui::BeginGroup();

		// Background
		drawList->AddRectFilled(origin, origin + size, BACKGROUND_COLOR);

		// Header
		drawList->AddRectFilled(origin, ImVec2(origin.x + size.x, origin.y + ITEM_HEIGHT), HEADER_COLOR);

		// Groups
		uint32_t legendBeginY = ITEM_HEIGHT;
		uint32_t entryIndex = 0;
		for (uint32_t group = 0; group < m_groups.size(); ++group) {
			uint32_t y = legendBeginY + entryIndex * ITEM_HEIGHT;
			// Legend
			ExpandButton(origin + ImVec2(LEGEND_LENGTH - 20, y + 8), m_groups[group].expanded);
			drawList->AddText(origin + ImVec2(LEGEND_TEXT_OFFSET, y), RULER_MARK_COLOR, m_groups[group].name.c_str());
			// Strip
			uint32_t color = (entryIndex & 1) ? STRIP_COLOR_ODD : STRIP_COLOR_EVEN;
			drawList->AddRectFilled(
				origin + ImVec2(LEGEND_LENGTH, y),
				origin + ImVec2(size.x, y + ITEM_HEIGHT),
				color);
			++entryIndex;
			// Group items
			if (m_groups[group].expanded) {
				const auto& items = m_groups[group].items;
				for (uint32_t item = 0; item < items.size(); ++item) {
					uint32_t y = legendBeginY + entryIndex * ITEM_HEIGHT;
					drawList->AddText(origin + ImVec2(LEGEND_TEXT_OFFSET, y), RULER_MARK_COLOR, items[item].name.c_str());
					// Strip
					uint32_t color = (entryIndex & 1) ? STRIP_COLOR_ODD : STRIP_COLOR_EVEN;
					drawList->AddRectFilled(
						origin + ImVec2(LEGEND_LENGTH, y),
						origin + ImVec2(size.x, y + ITEM_HEIGHT),
						color);
					++entryIndex;
				}
			}
		}

		// Ruler
		uint32_t rulerBeginX = LEGEND_LENGTH;
		uint32_t rulerEndX = size.x;
		for (uint32_t i = rulerBeginX, j = 0; i <= rulerEndX; i += RULER_SHORT_MARK_STEP, ++j) {
			if (j % RULER_LONG_MARK_MULTIPLIER == 0) {
				drawList->AddLine(
					origin + ImVec2(i, ITEM_HEIGHT), 
					origin + ImVec2(i, ITEM_HEIGHT - RULER_LONG_MARK_LENGTH), 
					RULER_MARK_COLOR);
				char buf[16];
				std::snprintf(buf, sizeof(buf), "%i", j);
				drawList->AddText(origin + ImVec2(i + RULER_TEXT_OFFSET, 0), RULER_MARK_COLOR, buf);
			}
			else {
				drawList->AddLine(
					origin + ImVec2(i, ITEM_HEIGHT), 
					origin + ImVec2(i, ITEM_HEIGHT - RULER_SHORT_MARK_LENGTH), 
					RULER_MARK_COLOR);
			}

			// Vertical line
			uint32_t lineBeginY = ITEM_HEIGHT;
			uint32_t lineEndY = lineBeginY + ITEM_HEIGHT * entryIndex;
			drawList->AddLine(
				origin + ImVec2(i, lineBeginY),
				origin + ImVec2(i, lineEndY),
				VERTICAL_MARK_COLOR);
		}

		// Dopes

		ImGui::EndGroup();
		ImGui::End();
	}
}
