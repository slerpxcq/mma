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

	static ImVec2 GlobalToRectNormalized(ImVec2 rectMin, ImVec2 rectMax, ImVec2 global)
	{
		return ImVec2(
			(global.x - rectMin.x) / (rectMax.x - rectMin.x),
			(global.y - rectMin.y) / (rectMax.y - rectMin.y));
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

	static uint32_t TextColor(bool enabled)
	{
		return enabled ? 0xffc0c0c0 : 0xff606060;
	}

	void CurveEditor::DrawRows()
	{
		ImVec2 textPos = ImVec2(LEGEND_TEXT_OFFSET, RULER_HEIGHT);
		ImVec2 buttonSize = ImVec2(LEGEND_LENGTH - textPos.x, RULER_HEIGHT);

		m_drawList->AddText(m_canvasOrigin + textPos, TextColor(m_enabledAxes & TRANSLATION_X_BIT), "Translation X");
		ImGui::SetCursorScreenPos(m_canvasOrigin + textPos);
		if (ImGui::InvisibleButton(GenButtonId(), buttonSize)) {
			m_enabledAxes ^= TRANSLATION_X_BIT;
		}

		textPos.y += RULER_HEIGHT;
		m_drawList->AddText(m_canvasOrigin + textPos, TextColor(m_enabledAxes & TRANSLATION_Y_BIT), "Translation Y");
		ImGui::SetCursorScreenPos(m_canvasOrigin + textPos);
		if (ImGui::InvisibleButton(GenButtonId(), buttonSize)) {
			m_enabledAxes ^= TRANSLATION_Y_BIT;
		}

		textPos.y += RULER_HEIGHT;
		m_drawList->AddText(m_canvasOrigin + textPos, TextColor(m_enabledAxes & TRANSLATION_Z_BIT), "Translation Z");
		ImGui::SetCursorScreenPos(m_canvasOrigin + textPos);
		if (ImGui::InvisibleButton(GenButtonId(), buttonSize)) {
			m_enabledAxes ^= TRANSLATION_Z_BIT;
		}

		textPos.y += RULER_HEIGHT;
		m_drawList->AddText(m_canvasOrigin + textPos, TextColor(m_enabledAxes & ROTATION_X_BIT), "Rotation X");
		ImGui::SetCursorScreenPos(m_canvasOrigin + textPos);
		if (ImGui::InvisibleButton(GenButtonId(), buttonSize)) {
			m_enabledAxes ^= ROTATION_X_BIT;
		}

		textPos.y += RULER_HEIGHT;
		m_drawList->AddText(m_canvasOrigin + textPos, TextColor(m_enabledAxes & ROTATION_Y_BIT), "Rotation Y");
		ImGui::SetCursorScreenPos(m_canvasOrigin + textPos);
		if (ImGui::InvisibleButton(GenButtonId(), buttonSize)) {
			m_enabledAxes ^= ROTATION_Y_BIT;
		}

		textPos.y += RULER_HEIGHT;
		m_drawList->AddText(m_canvasOrigin + textPos, TextColor(m_enabledAxes & ROTATION_Z_BIT), "Rotation Z");
		ImGui::SetCursorScreenPos(m_canvasOrigin + textPos);
		if (ImGui::InvisibleButton(GenButtonId(), buttonSize)) {
			m_enabledAxes ^= ROTATION_Z_BIT;
		}
	}

	void CurveEditor::DrawRotationCurve(const Animation::BoneKeyframe& keyframe, const Animation::BoneKeyframe& next)
	{
		static constexpr uint32_t MAX_SEGMENT = 32;
		static ImVec2 points[3][MAX_SEGMENT];

		for (uint32_t segment = 0; segment < MAX_SEGMENT; ++segment) {
			float target = (float)segment / (MAX_SEGMENT - 1);
			float t = next.bezier.Eval(3, target);
			glm::quat q = glm::slerp(keyframe.transform.rotation, next.transform.rotation, t);
			glm::vec3 euler = glm::eulerAngles(q);

			for (uint32_t i = 0; i < 3; ++i) {
				float x_glob = glm::mix((float)keyframe.frame, (float)next.frame, target);
				float y_glob = euler[i];
				points[i][segment] = m_canvasOrigin + 
					GlobalToRectNormalized(m_rectMin, m_rectMax, ImVec2(x_glob, y_glob)) * m_canvasSize;
			}
		}

		for (uint32_t axis = 0; axis < 3; ++axis) {
			if (!(m_enabledAxes & (1 << (axis+3))))
				continue;
			m_drawList->AddPolyline(points[axis], MAX_SEGMENT, rotationColors[axis], ImDrawFlags_None, 2.0f);
		}
	}

	void CurveEditor::DrawTranslationCurve(const Animation::BoneKeyframe& keyframe, const Animation::BoneKeyframe& next)
	{
		for (uint32_t axis = 0; axis < 3; ++axis) {
			if (!(m_enabledAxes & (1 << axis)))
				continue;

			float x_glob = keyframe.frame;
			float x_next_glob = next.frame;
			float y_glob = keyframe.transform.translation[axis];
			float y_next_glob = next.transform.translation[axis];

			ImVec2 p1 = GlobalToRectNormalized(m_rectMin, m_rectMax, ImVec2(x_glob, y_glob)) * m_canvasSize;
			ImVec2 p4 = GlobalToRectNormalized(m_rectMin, m_rectMax, ImVec2(x_next_glob, y_next_glob)) * m_canvasSize;

			auto& handles = next.bezier.GetHandles();
			ImVec2 p2 = ImVec2(handles[axis][0].x / 127.f, handles[axis][0].y / 127.f);
			p2 = p1 + (p4 - p1) * p2;
			ImVec2 p3 = ImVec2(handles[axis][1].x / 127.f, handles[axis][1].y / 127.f);
			p3 = p1 + (p4 - p1) * p3;

			m_drawList->AddBezierCubic(
				m_canvasOrigin + p1,
				m_canvasOrigin + p2,
				m_canvasOrigin + p3,
				m_canvasOrigin + p4,
				translationColors[axis], 2.0f);
		}
	}

	void CurveEditor::OnUIRender()
	{
		ImGui::Begin("Curve Editor", nullptr,
			ImGuiWindowFlags_NoScrollbar |
			ImGuiWindowFlags_NoScrollWithMouse);

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

		if (m_container != nullptr) {
			auto keyframe = LowerBound(*m_container, m_rectMin.x);
			for (; keyframe != m_container->end() && keyframe->frame <= m_rectMax.x; ++keyframe) {
				auto next = std::next(keyframe);
				if (next != m_container->end()) {
					DrawRotationCurve(*keyframe, *next);
					DrawTranslationCurve(*keyframe, *next);
				}
			}
		}

		ImGui::EndGroup();

		ImGuiIO& io = ImGui::GetIO();

		/* Pan */
		if (ImGui::IsMouseDragging(ImGuiMouseButton_Middle) && !m_anyHandleActivated) {
			static constexpr float PAN_SPEED = 0.001;
            ImVec2 pan = io.MouseDelta * (m_rectMax - m_rectMin) * PAN_SPEED;
            m_rectMin = m_rectMin - pan;
            m_rectMax = m_rectMax - pan;
		}

		/* Zoom */
		if (ImGui::IsWindowHovered()) {
			 static constexpr float SCROLL_SPEED = 0.1;

             float wheel = -io.MouseWheel;

			 ImVec2 delta;
			 if (ImGui::IsKeyDown(ImGuiKey_LeftCtrl))
			 	delta = ImVec2(1, 0);
			 else if (ImGui::IsKeyDown(ImGuiKey_LeftShift))
			 	delta = ImVec2(0, 1);
			 else 
			 	delta = ImVec2(1, 1);
             delta = delta * wheel * SCROLL_SPEED * (m_rectMax - m_rectMin);

             ImVec2 newMin = m_rectMin - delta;
             ImVec2 newMax = m_rectMax + delta;
             if (newMax.x - newMin.x > 1 && newMax.y - newMin.y > 1) {
			 	m_rectMin = newMin;
			 	m_rectMax = newMax;
             }
		}

		/* Pan view */
		if (ImGui::IsMouseClicked(ImGuiMouseButton_Left) && !m_anyHandleActivated) {
			m_selectedKeyframe.keyframe = nullptr;
			m_selectedKeyframe.axis = -1;
		}

		m_drawList->PopClipRect();
		ImGui::End();

		m_anyHandleActivated = false;
	}
}
