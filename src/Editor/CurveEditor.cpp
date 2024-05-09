#include "mmpch.hpp"
#include "CurveEditor.hpp"

#include "Core/Utility/Type.hpp"
#include "Core/MM/Model/Model.hpp"
#include "Core/MM/Animation/Animation.hpp"

#include "Properties.hpp"

namespace mm
{
	void CurveEditor::OnItemSelected(const EditorEvent::ItemSelected& e)
	{
		if (e.type == Properties::TYPE_BONE) {
			/* Get the container, how? */
		}
	}

	void CurveEditor::OnMouseScrolled(const Event::MouseScrolled& e)
	{
		if (!m_hovered)
			return;

		m_yGain += e.delta;
		m_yGain = std::max(1.0f, m_yGain);
	}

	void CurveEditor::DrawDiamond(const ImVec2& center, float radius, float outlineSize, uint32_t outlineColor, uint32_t fillColor)
	{
		ImVec2 p0 = m_canvasOrigin + ImVec2(center.x - radius, center.y);
		ImVec2 p1 = m_canvasOrigin + ImVec2(center.x, center.y - radius);
		ImVec2 p2 = m_canvasOrigin + ImVec2(center.x + radius, center.y);
		ImVec2 p3 = m_canvasOrigin + ImVec2(center.x, center.y + radius);
		m_drawList->AddQuad(p0, p1, p2, p3, outlineColor, outlineSize);
		m_drawList->AddQuadFilled(p0, p1, p2, p3, fillColor);
	}

	void CurveEditor::DrawScale()
	{
		/* Ruler */
		float rulerBeginX = LEGEND_LENGTH;
		float rulerEndX = m_canvasSize.x;

		/* Ruler background */
		m_drawList->AddRectFilled(m_canvasOrigin, ImVec2(m_canvasOrigin.x + m_canvasSize.x, m_canvasOrigin.y + RULER_HEIGHT), HEADER_COLOR);

		/* Vertical scale */
		for (float x = LEGEND_LENGTH; x < m_canvasSize.x; x += 20.f) {
			m_drawList->AddLine(
				m_canvasOrigin + ImVec2(x, RULER_HEIGHT), 
				m_canvasOrigin + ImVec2(x, m_canvasSize.y), 0x20c0c0c0, 1.0f);
		}
		/* Horizontal scale */
		for (float y = 0; y < m_canvasSize.y; y += 20.f) {
			m_drawList->AddLine(
				m_canvasOrigin + ImVec2(LEGEND_LENGTH, y), 
				m_canvasOrigin + ImVec2(m_canvasSize.x, y), 0x20c0c0c0, 1.0f);
		}
	}

	void CurveEditor::DrawRows()
	{
	}

	void CurveEditor::OnUIRender()
	{
		ImGui::Begin("Curve Editor", nullptr,
			ImGuiWindowFlags_NoScrollbar |
			ImGuiWindowFlags_NoScrollWithMouse);

		ImGui::PushItemWidth(150);
		if (ImGui::InputInt("Min", &m_minFrame)) {

		}
		ImGui::SameLine();
		if (ImGui::InputInt("Max", &m_maxFrame)) {
		}
		ImGui::PopItemWidth();
		m_maxFrame = std::max(m_minFrame + 10, m_maxFrame);


		m_hovered = ImGui::IsWindowHovered();
		m_drawList = ImGui::GetWindowDrawList();
		m_canvasMin = ImGui::GetCursorPos();
		m_canvasMax = ImGui::GetWindowContentRegionMax();
		m_canvasOrigin = ImGui::GetWindowPos() + m_canvasMin;
		m_canvasSize = m_canvasMax - m_canvasMin;

		ImGui::BeginGroup();

		/*************************************** BEGIN DRAWING ***************************************/

		/* Background */
		m_drawList->AddRectFilled(m_canvasOrigin, m_canvasOrigin + m_canvasSize, BACKGROUND_COLOR);

		DrawScale();
		DrawRows();

		if (m_container != nullptr) {
			auto keyframe = LowerBound(*m_container, m_minFrame);
			float y0 = m_canvasSize.y * 0.5f;
			for (; keyframe != m_container->end() && keyframe->frame < m_maxFrame; ++keyframe) {
				auto next = std::next(keyframe);
				for (uint32_t axis = 0; axis < 3; ++axis) {
					float x = LEGEND_LENGTH + (m_canvasSize.x - LEGEND_LENGTH) * (float)(keyframe->frame - m_minFrame) / (m_maxFrame - m_minFrame);
					float y = y0 + keyframe->transform.translation[axis] * m_yGain;

					DrawDiamond(ImVec2(x, y), 
						DOPE_RADIUS, DOPE_OUTLINE_SIZE, DOPE_OUTLINE_COLOR, DOPE_FILL_COLOR);

					if (next != m_container->end()) {
						float nextX = LEGEND_LENGTH + (m_canvasSize.x - LEGEND_LENGTH) * (float)(next->frame - m_minFrame) / (m_maxFrame - m_minFrame);
						float nextY = y0 + next->transform.translation[axis] * m_yGain;

						ImVec2 p1 = ImVec2(x, y);
						ImVec2 p4 = ImVec2(nextX, nextY);

						auto& handles = keyframe->bezier.GetHandles();
						ImVec2 p2 = ImVec2(handles[axis][0].x / 127.f, handles[axis][0].y / 127.f);
						p2 = p1 + (p4 - p1) * p2;
						ImVec2 p3 = ImVec2(handles[axis][1].x / 127.f, handles[axis][1].y / 127.f);
						p3 = p1 + (p4 - p1) * p3;

						const uint32_t colors[3] = {
							0xbf0000ff,
							0xbf00ff00,
							0xbfff0000
						};

						m_drawList->AddBezierCubic(
							m_canvasOrigin + p1,
							m_canvasOrigin + p2,
							m_canvasOrigin + p3,
							m_canvasOrigin + p4,
							colors[axis], 2.0f);
					}
				}
			}
		}
		ImGui::EndGroup();

		ImGui::End();
	}
}
