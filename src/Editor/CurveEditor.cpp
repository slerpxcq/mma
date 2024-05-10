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
		m_buttonIndex = 0;

		ImGui::BeginGroup();

		/*************************************** BEGIN DRAWING ***************************************/

		/* Background */
		m_drawList->AddRectFilled(m_canvasOrigin, m_canvasOrigin + m_canvasSize, BACKGROUND_COLOR);
		m_drawList->PushClipRect(m_canvasOrigin, m_canvasOrigin + m_canvasSize);

		DrawScale();
		DrawRows();

		const uint32_t translationColors[3] = {
			0xbf0000ff,
			0xbf00ff00,
			0xbfff0000
		};

		const uint32_t rotationColors[3] = {
			0xbf4040ff,
			0xbf40ff40,
			0xbfff4040
		};

		if (m_container != nullptr) {
			auto keyframe = LowerBound(*m_container, m_minFrame);
			float y0 = m_canvasSize.y * 0.5f;

			for (; keyframe != m_container->end() && keyframe->frame < m_maxFrame; ++keyframe) {
				float x = LEGEND_LENGTH + (m_canvasSize.x - LEGEND_LENGTH) * (float)(keyframe->frame - m_minFrame) / (m_maxFrame - m_minFrame) - m_coordOrigin.x;
				auto next = std::next(keyframe);
				if (next != m_container->end()) {
					float nextX = LEGEND_LENGTH + (m_canvasSize.x - LEGEND_LENGTH) * (float)(next->frame - m_minFrame) / (m_maxFrame - m_minFrame) - m_coordOrigin.x;

					/* ------------------ Rotation curve ------------------ */
					static constexpr uint32_t MAX_SEGMENT = 32;
					static ImVec2 points[3][MAX_SEGMENT];

					for (uint32_t segment = 0; segment < MAX_SEGMENT; ++segment) {
						float target = (float)segment / (MAX_SEGMENT - 1);
						float t = next->bezier.Eval(3, target);
						glm::quat q = glm::slerp(keyframe->transform.rotation, next->transform.rotation, t);
						glm::vec3 euler = glm::eulerAngles(q);

						for (uint32_t i = 0; i < 3; ++i) {
							points[i][segment] = m_canvasOrigin + ImVec2(x + target * (nextX - x), y0 + euler[i] * m_yGain - m_coordOrigin.y);
						}
					}

					for (uint32_t i = 0; i < 3; ++i) {
						m_drawList->AddPolyline(points[i], MAX_SEGMENT, rotationColors[i], ImDrawFlags_None, 2.0f);
					}

					/* ------------------ Translation curve ------------------ */
					for (uint32_t axis = 0; axis < 3; ++axis) {
						float y = y0 + keyframe->transform.translation[axis] * m_yGain - m_coordOrigin.y;

						ImGui::SetCursorScreenPos(m_canvasOrigin + ImVec2(x - DOPE_RADIUS, y - DOPE_RADIUS));
						if (ImGui::InvisibleButton(GenButtonId(), ImVec2(DOPE_RADIUS * 2, DOPE_RADIUS * 2))) {
							SelectedDope s = {};
							s.axis = axis;
							s.keyframe = &*keyframe;
							m_selectedKeyframe = s;
						}

						bool selected = &*keyframe == m_selectedKeyframe.keyframe && axis == m_selectedKeyframe.axis;
						m_drawList->AddCircleFilled(m_canvasOrigin + ImVec2(x, y), DOPE_RADIUS, 0xffc0c0c0);
						if (selected)
							m_drawList->AddCircle(m_canvasOrigin + ImVec2(x, y), DOPE_RADIUS, DOPE_OUTLINE_COLOR, 0, 2.f);

						float nextY = y0 + next->transform.translation[axis] * m_yGain - m_coordOrigin.y;

						ImVec2 p1 = ImVec2(x, y);
						ImVec2 p4 = ImVec2(nextX, nextY);

						auto& handles = next->bezier.GetHandles();
						ImVec2 p2 = ImVec2(handles[axis][0].x / 127.f, handles[axis][0].y / 127.f);
						p2 = p1 + (p4 - p1) * p2;
						ImVec2 p3 = ImVec2(handles[axis][1].x / 127.f, handles[axis][1].y / 127.f);
						p3 = p1 + (p4 - p1) * p3;

						if (selected) {
							/* Draw the Bezier bound rectangle */
							m_drawList->AddRectFilled(m_canvasOrigin + p1, m_canvasOrigin + p4, 0x20c0c0c0);

							ImGui::SetCursorScreenPos(m_canvasOrigin + p2 - ImVec2(DOPE_RADIUS, DOPE_RADIUS));
							ImGui::InvisibleButton(GenButtonId(), ImVec2(DOPE_RADIUS * 2, DOPE_RADIUS * 2));
							static ImVec2 p2OnActivated;
							if (ImGui::IsItemActivated()) {
								p2OnActivated = p2;
							}
							if (ImGui::IsItemActive() && ImGui::IsMouseDragging(ImGuiMouseButton_Left)) {
								m_anyHandleActivated = true;
								p2 = p2OnActivated + ImGui::GetMouseDragDelta(ImGuiMouseButton_Left);
								float maxY = std::max(p1.y, p4.y);
								float minY = std::min(p1.y, p4.y);
								p2.x = std::clamp(p2.x, p1.x, p4.x);
								p2.y = std::clamp(p2.y, minY, maxY);
								ImVec2 handleA = (p2 - p1) / (p4 - p1) * 127.f;
								handles[axis][0].x = std::roundf(handleA.x);
								handles[axis][0].y = std::roundf(handleA.y);
							}

							ImGui::SetCursorScreenPos(m_canvasOrigin + p3 - ImVec2(DOPE_RADIUS, DOPE_RADIUS));
							ImGui::InvisibleButton(GenButtonId(), ImVec2(DOPE_RADIUS * 2, DOPE_RADIUS * 2));
							static ImVec2 p3OnActivated;
							if (ImGui::IsItemActivated()) {
								p3OnActivated = p3;
							}
							if (ImGui::IsItemActive() && ImGui::IsMouseDragging(ImGuiMouseButton_Left)) {
								m_anyHandleActivated = true;
								p3 = p3OnActivated + ImGui::GetMouseDragDelta(ImGuiMouseButton_Left);
								float maxY = std::max(p1.y, p4.y);
								float minY = std::min(p1.y, p4.y);
								p3.x = std::clamp(p3.x, p1.x, p4.x);
								p3.y = std::clamp(p3.y, minY, maxY);
								ImVec2 handleB = (p3 - p1) / (p4 - p1) * 127.f;
								handles[axis][1].x = std::roundf(handleB.x);
								handles[axis][1].y = std::roundf(handleB.y);
							}

							/* Draw handles and handle buttons */
							m_drawList->AddLine(m_canvasOrigin + p1, m_canvasOrigin + p2, 0xffc0c0c0);
							m_drawList->AddLine(m_canvasOrigin + p3, m_canvasOrigin + p4, 0xffc0c0c0);
							m_drawList->AddCircle(m_canvasOrigin + p2, DOPE_RADIUS, 0xffc0c0c0);
							m_drawList->AddCircle(m_canvasOrigin + p3, DOPE_RADIUS, 0xffc0c0c0);
						}

						m_drawList->AddBezierCubic(
							m_canvasOrigin + p1,
							m_canvasOrigin + p2,
							m_canvasOrigin + p3,
							m_canvasOrigin + p4,
							translationColors[axis], 2.0f);
					}
				}
			}
		}
		ImGui::EndGroup();

		/* Pan view */
		static bool dragged;
		static ImVec2 begin;
		bool dragging = ImGui::IsMouseDragging(ImGuiMouseButton_Left);
		if (dragged != dragging && !m_anyHandleActivated) {
			if (dragging) 
				begin = m_coordOrigin;
			dragged = dragging;
		}
		if (m_hovered && dragging && !m_anyHandleActivated) {
			m_coordOrigin = begin - ImGui::GetMouseDragDelta(ImGuiMouseButton_Left);
		}

		//if (ImGui::IsMouseClicked(ImGuiMouseButton_Left) && !m_anyHandleActivated) {
		//	m_selectedKeyframe.keyframe = nullptr;
		//	m_selectedKeyframe.axis = -1;
		//}

		m_drawList->PopClipRect();
		ImGui::End();

		m_anyHandleActivated = false;
	}
}
