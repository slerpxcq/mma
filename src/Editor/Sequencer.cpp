#include "mmpch.hpp"
#include "Sequencer.hpp"

#include "Core/MM/Model/Model.hpp"

#include "Core/Utility/Type.hpp"
#include "Core/App/Input.hpp"

namespace mm
{
	void Sequencer::DrawExpandButton(uint32_t rowIndex, float offsetX, bool& expanded)
	{
		ImVec2 pos = ImVec2(offsetX, rowIndex * ROW_HEIGHT + 8);

		ImGui::SetCursorScreenPos(m_canvasOrigin + pos);
		if (ImGui::InvisibleButton(GenButtonId(), BUTTON_SIZE)) {
			expanded = !expanded;
		}

		ImVec2 max = pos + BUTTON_SIZE;
		ImVec2 mid = pos + 0.5f * BUTTON_SIZE;
		if (!expanded) {
			/* Lower triangle */
			m_drawList->AddTriangle(
				m_canvasOrigin + ImVec2(mid.x, pos.y), 
				m_canvasOrigin + ImVec2(max.x, mid.y),
				m_canvasOrigin + ImVec2(mid.x, max.y), BUTTON_COLOR);
		}
		else {
			/* Upper triangle */
			static constexpr float RATIO = 0.8f;
			m_drawList->AddTriangleFilled(
				m_canvasOrigin + ImVec2(max.x, max.y - RATIO * BUTTON_SIZE.y),
				m_canvasOrigin + ImVec2(max.x, max.y),
				m_canvasOrigin + ImVec2(max.x - RATIO * BUTTON_SIZE.x, max.y),
				BUTTON_COLOR);
		}
	}

	/* Use ImGui events */
	void Sequencer::OnMouseScrolled(const Event::MouseScrolled& e)
	{
		if (!m_hovered)
			return;

		m_rowStart += e.delta;
		m_rowStart = std::clamp(m_rowStart, -m_totalRowCount + 1, 1);
	}

	void Sequencer::DrawDiamond(const ImVec2& center, float radius, float outlineSize, uint32_t outlineColor, uint32_t fillColor)
	{
		ImVec2 p0 = m_canvasOrigin + ImVec2(center.x - radius, center.y);
		ImVec2 p1 = m_canvasOrigin + ImVec2(center.x, center.y - radius);
		ImVec2 p2 = m_canvasOrigin + ImVec2(center.x + radius, center.y);
		ImVec2 p3 = m_canvasOrigin + ImVec2(center.x, center.y + radius);
		m_drawList->AddQuad(p0, p1, p2, p3, outlineColor, outlineSize);
		m_drawList->AddQuadFilled(p0, p1, p2, p3, fillColor);
	}

	template <typename T>
	void Sequencer::DrawRowHeader(T& row, bool expandable, float textOffset)
	{
		if (row.rowIndex < 1)
			return;

		float y = row.rowIndex * ROW_HEIGHT;
		m_drawList->AddText(m_canvasOrigin + ImVec2(LEGEND_TEXT_OFFSET + textOffset, y), RULER_MARK_COLOR, row.name.c_str());
		if (expandable) 
			DrawExpandButton(row.rowIndex, textOffset - 5, row.expanded);
	}

	template <typename T>
	decltype(auto) Sequencer::GetFirstKeyframeToDraw(std::vector<T>& keyframeList)
	{
		auto it = std::lower_bound(
			keyframeList.begin(), 
			keyframeList.end(), 
			m_minFrame, 
			[](const T& lhs, uint32_t rhs) {
				return lhs.frame < rhs;
			});
		return it;
	}

	bool Sequencer::IsKeyframeSelected(Animation::Keyframe& keyframe)
	{
		auto it = m_selectedKeyframes.find(&keyframe);
		return it != m_selectedKeyframes.end();
	}

	void Sequencer::DrawGroupDope(const Group& group)
	{
	}

	template <typename T>
	void Sequencer::DrawItemDope(const Item& item, std::vector<T>& keyframeList)
	{
		/* The first row is the ruler, don't draw on it! */
		if (item.rowIndex < 1)
			return;

		auto currKeyframe = GetFirstKeyframeToDraw(keyframeList);

		for (; currKeyframe != keyframeList.end() && currKeyframe->frame < m_maxFrame; ++currKeyframe) {
			float x = LEGEND_LENGTH + ROW_HEIGHT / 2;
			float y = item.rowIndex * ROW_HEIGHT;
			uint32_t column = currKeyframe->frame - m_minFrame;

			ImVec2 dopePos = ImVec2(x + column * COLUMN_WIDTH, y + ROW_HEIGHT / 2);
			ImVec2 buttonSize = 2.f * ImVec2(DOPE_RADIUS, DOPE_RADIUS);
			ImVec2 buttonPos = dopePos - 0.5f * buttonSize;

			bool startDragging = !m_lastFrameMouseDragged && m_thisFrameMouseDragging;
			if (startDragging)
				;
			ImGui::SetCursorScreenPos(m_canvasOrigin + buttonPos);
			if (ImGui::InvisibleButton(GenButtonId(), buttonSize)) {
				if (ImGui::IsKeyDown(ImGuiKey_LeftCtrl) && IsKeyframeSelected(*currKeyframe) && !startDragging)
					m_selectedKeyframes.erase(&(*currKeyframe));
				else
					m_selectedKeyframes.insert(&(*currKeyframe));
			}
			if (ImGui::IsMouseClicked(ImGuiMouseButton_Left) && IsKeyframeSelected(*currKeyframe) && ImGui::IsItemHovered()) {
				m_mouseClickedOnItem = true;
			}
			if (m_selectionBox.CheckIntersection(m_canvasOrigin + buttonPos, DOPE_RADIUS)) {
				m_selectedKeyframes.insert(&(*currKeyframe));
			}
			/* Detect start dragging on a selected dope */
			/* Should be one shot */
			/* 0. Not dragging now */
			/* 1. It is selected */
			/* 2. Mouse on it */
			/* 3. Mouse start dragging */
			if (!m_dragging && IsKeyframeSelected(*currKeyframe) && ImGui::IsItemHovered() && startDragging) {
				m_dragging = true;
				m_mouseXWhenStartDragging = ImGui::GetMousePos().x;
				/* Store frame number of all keyframes being dragged */
				for (auto& keyframe : m_selectedKeyframes) {
					m_keyframeFrameNumberMap.insert({ keyframe, keyframe->frame });
				}
				MM_INFO("Start dragging");
			}

			/* if the keyframe is in selected set or selected by the box */
			if (IsKeyframeSelected(*currKeyframe))
				DrawDiamond(dopePos, DOPE_RADIUS, DOPE_OUTLINE_SIZE_SELECTED, DOPE_OUTLINE_COLOR_SELECTED, DOPE_FILL_COLOR);
			else
				DrawDiamond(dopePos, DOPE_RADIUS, DOPE_OUTLINE_SIZE, DOPE_OUTLINE_COLOR, DOPE_FILL_COLOR);
		}
	}

	void Sequencer::CurveEditor(Item& item)
	{
		/* Curve editor */
		ImVec2 editorOrigin = ImVec2(LEGEND_LENGTH, (item.rowIndex + 1) * ROW_HEIGHT);
		float editorHeight = CURVE_EDITOR_ROW_COUNT * ROW_HEIGHT;
		float midY = editorOrigin.y + editorHeight * 0.5f;


		// NOTE: only bone keyframes are expandable.
		MM_ASSERT(item.type == PMXFile::CLUSTER_BONE);
		auto& keyframeList = m_model->GetAnim().GetBoneKeyframeMatrix()[item.index];
		auto it = std::lower_bound(
			keyframeList.begin(),
			keyframeList.end(),
			m_minFrame,
			[](const Animation::Keyframe& lhs, uint32_t rhs) {
				return lhs.frame < rhs;
			});
			//[](uint32_t lhs, const Animation::Keyframe& rhs) {
			//	return lhs < rhs.frame;
			//});

		/* Bezier */
		/* Background */
		ImVec2 pos = ImVec2(LEGEND_LENGTH / 2, midY);
		ImVec2 p1 = pos - ImVec2(BEZIER_EDITOR_SIZE/2, BEZIER_EDITOR_SIZE/2);
		ImVec2 p2 = pos + ImVec2(BEZIER_EDITOR_SIZE/2, BEZIER_EDITOR_SIZE/2);
		m_drawList->AddRectFilled(m_canvasOrigin + p1, m_canvasOrigin + p2, IM_COL32(32, 32, 32, 255));
		m_drawList->AddRect(m_canvasOrigin + p1, m_canvasOrigin + p2, VERTICAL_MARK_COLOR, 0.0f, 0, 3.0f);
		/* Grid */
		for (int32_t x = pos.x - BEZIER_EDITOR_SIZE / 2; x <= pos.x + BEZIER_EDITOR_SIZE / 2; x += BEZIER_EDITOR_SIZE / BEZIER_EDITOR_GRID_COUNT)
			m_drawList->AddLine(m_canvasOrigin + ImVec2(x, p1.y), m_canvasOrigin + ImVec2(x, p2.y), VERTICAL_MARK_COLOR);
		//for (int32_t y = pos.y - BEZIER_EDITOR_SIZE / 2; y <= pos.y + BEZIER_EDITOR_SIZE / 2; y += BEZIER_EDITOR_SIZE / BEZIER_EDITOR_GRID_COUNT)
		//	m_drawList->AddLine(m_canvasOrigin + ImVec2(p1.x, y), m_canvasOrigin + ImVec2(p2.x, y), VERTICAL_MARK_COLOR);
		/* Handle */

		/* Curve */
		m_drawList->PushClipRect(m_canvasOrigin + editorOrigin, m_canvasOrigin + ImVec2(m_canvasSize.x, editorOrigin.y + editorHeight));
		//if (it != keyframeList.begin())
		//	it -= 1;
		for (; it != keyframeList.end() && it->frame < m_maxFrame; ++it) {
			// for all axes
			static constexpr uint32_t axisColor[] = {
				IM_COL32(255, 0, 0, 128), // X->R
				IM_COL32(0, 255, 0, 128), // Y->G
				IM_COL32(0, 0, 255, 128)}; // Z->B

			for (uint32_t axis = 0; axis < 3; ++axis) {
				float y = midY + it->transform.translation[axis] * Y_GAIN;
				float startX = LEGEND_LENGTH + ROW_HEIGHT / 2;
				uint32_t column = it->frame - m_minFrame;
				ImVec2 diamondPos = ImVec2(startX + column * COLUMN_WIDTH, y + ROW_HEIGHT / 2);
				DrawDiamond(diamondPos, POINT_RADIUS, POINT_OUTLINE_SIZE, POINT_OUTLINE_COLOR, POINT_FILL_COLOR);
				if (it + 1 != keyframeList.end()) {
					ImVec2 bezP0 = diamondPos;
					float nextY = midY + (it + 1)->transform.translation[axis] * Y_GAIN;
					uint32_t nextColumn = (it + 1)->frame - m_minFrame;
					ImVec2 bezP3 = ImVec2(startX + nextColumn * COLUMN_WIDTH, nextY + ROW_HEIGHT / 2);
					ImVec2 delta = bezP3 - bezP0;
					ImVec2 vmdP1 = ImVec2(it->bezier.GetHandles()[axis][0].x, it->bezier.GetHandles()[axis][0].y);
					ImVec2 bezP1 = ((vmdP1 * (1.f / 127)) * delta) + bezP0;
					ImVec2 vmdP2 = ImVec2(it->bezier.GetHandles()[axis][1].x, it->bezier.GetHandles()[axis][1].y);
					ImVec2 bezP2 = ((vmdP2 * (1.f / 127)) * delta) + bezP0;
					m_drawList->AddBezierCubic(
						m_canvasOrigin + bezP0, 
						m_canvasOrigin + bezP1, 
						m_canvasOrigin + bezP2, 
						m_canvasOrigin + bezP3,
						axisColor[axis], 3.0f);
				}
			}
		}

		m_drawList->PopClipRect();
	}

	void Sequencer::DrawStrip(uint32_t row)
	{
		if (row >= m_visibleRowCount)
			return;

		uint32_t stripColor = (row & 1) ? STRIP_COLOR_ODD : STRIP_COLOR_EVEN;
		float y = row * ROW_HEIGHT;
		m_drawList->AddRectFilled(
			m_canvasOrigin + ImVec2(LEGEND_LENGTH, y),
			m_canvasOrigin + ImVec2(m_canvasSize.x, y + ROW_HEIGHT),
			stripColor);
	}

	void Sequencer::DrawScale()
	{
		/* Ruler */
		float rulerBeginX = LEGEND_LENGTH + ROW_HEIGHT / 2;
		float rulerEndX = m_canvasSize.x;
		uint32_t column = m_minFrame;
		uint32_t columnCount = 0;
		m_drawList->AddRectFilled(m_canvasOrigin, ImVec2(m_canvasOrigin.x + m_canvasSize.x, m_canvasOrigin.y + ROW_HEIGHT), HEADER_COLOR);
		for (uint32_t rulerX = rulerBeginX; rulerX <= rulerEndX; rulerX += COLUMN_WIDTH, ++column, ++columnCount) {
			float lineBeginY = ROW_HEIGHT;
			float lineEndY = ROW_HEIGHT * m_visibleRowCount;
			/* Button for selecting frame */
			ImVec2 buttonPos = ImVec2(rulerX - COLUMN_WIDTH / 2, 0);
			ImVec2 buttonSize = ImVec2(COLUMN_WIDTH, ROW_HEIGHT);
			ImGui::SetCursorScreenPos(m_canvasOrigin + buttonPos);
			if (ImGui::InvisibleButton(GenButtonId(), buttonSize)) {
				m_selectedColumn = column;
				m_frameCounter.Set(column);
				UpdateAnim();
			}

			/* Draw current frame mark (red vertical line) */
			if (column == m_selectedColumn) {
				m_drawList->AddLine(
					m_canvasOrigin + ImVec2(rulerX, lineBeginY),
					m_canvasOrigin + ImVec2(rulerX, lineEndY),
					IM_COL32(255, 0, 0, 64), ROW_HEIGHT / 2);
			}

			/* Rule long mark */
			if (column % RULER_LONG_MARK_MULTIPLIER == 0) {
				m_drawList->AddLine(
					m_canvasOrigin + ImVec2(rulerX, ROW_HEIGHT),
					m_canvasOrigin + ImVec2(rulerX, ROW_HEIGHT - RULER_LONG_MARK_LENGTH),
					RULER_MARK_COLOR);
				char buf[16];
				std::snprintf(buf, sizeof(buf), "%i", column);
				m_drawList->AddText(m_canvasOrigin + ImVec2(rulerX + RULER_TEXT_OFFSET, 0), RULER_MARK_COLOR, buf);
				m_drawList->AddLine(
					m_canvasOrigin + ImVec2(rulerX, lineBeginY),
					m_canvasOrigin + ImVec2(rulerX, lineEndY),
					VERTICAL_MARK_COLOR, 2.0f);
			}
			/* Ruler short mark */
			else {
				m_drawList->AddLine(
					m_canvasOrigin + ImVec2(rulerX, ROW_HEIGHT),
					m_canvasOrigin + ImVec2(rulerX, ROW_HEIGHT - RULER_SHORT_MARK_LENGTH),
					RULER_MARK_COLOR);
				m_drawList->AddLine(
					m_canvasOrigin + ImVec2(rulerX, lineBeginY),
					m_canvasOrigin + ImVec2(rulerX, lineEndY),
					VERTICAL_MARK_COLOR);
			}
		}
		m_maxFrame = m_minFrame + columnCount;
	}

	void Sequencer::DrawExpandedGroup(Group& group)
	{
		for (auto& item : group.items) {
			item.rowIndex = m_currRowIndex++;
			if (item.rowIndex >= m_visibleRowCount)
				break;
			Animation& anim = m_model->GetAnim();
			DrawRowHeader(item, false, 2 * INDENT_BASE);
			switch (item.type) {
			case PMXFile::CLUSTER_BONE:
				DrawItemDope(item, anim.GetBoneKeyframeMatrix()[item.index]);
				break;
			case PMXFile::CLUSTER_MORPH:
				DrawItemDope(item, anim.GetMorphKeyframeMatrix()[item.index]);
				break;
			}
		}
	}

	void Sequencer::DrawRows()
	{
		m_currRowIndex = m_rowStart;
		for (auto& group : m_groups) {
			group.rowIndex = m_currRowIndex++;
			if (group.rowIndex >= m_visibleRowCount)
				break;
			DrawRowHeader(group, true, INDENT_BASE);
			if (group.expanded) {
				DrawExpandedGroup(group);
			}
		}
		m_totalRowCount = m_currRowIndex - 1;
	}

	void Sequencer::OnUIRender()
	{

		ImGui::Begin("Sequencer", nullptr,
			ImGuiWindowFlags_NoScrollbar |
			ImGuiWindowFlags_NoScrollWithMouse);

		if (ImGui::Button("Play")) {
			m_playing = true;
		}
		ImGui::SameLine();
		if (ImGui::Button("Stop")) {
			m_playing = false;
		}
		ImGui::SameLine();
		static int32_t frame;
		if (ImGui::InputInt("Frame", &frame)) {
			if (frame < 0)
				frame = 0;
			m_frameCounter.Set(frame);
			UpdateAnim();
		}

		if (ImGui::SliderInt("Min frame", &m_minFrame, 0, 10000)) {
		}
		ImGui::SameLine();
		if (ImGui::InputInt(" ", &m_minFrame)) {
		}
		m_minFrame = std::clamp(m_minFrame, 0, 10000);

		/* Init states */
		m_hovered = ImGui::IsWindowHovered();
		m_drawList = ImGui::GetWindowDrawList();
		m_canvasMin = ImGui::GetCursorPos();
		m_canvasMax = ImGui::GetWindowContentRegionMax();
		m_canvasOrigin = ImGui::GetWindowPos() + m_canvasMin;
		m_canvasSize = m_canvasMax - m_canvasMin;
		m_buttonIndex = 0;
		m_visibleRowCount = m_canvasSize.y / ROW_HEIGHT;

		ImGui::BeginGroup();

		/* Background */
		m_drawList->AddRectFilled(m_canvasOrigin, m_canvasOrigin + m_canvasSize, BACKGROUND_COLOR);

		/* Strip */
		for (auto& group : m_groups) {
			DrawStrip(group.rowIndex);
			if (group.expanded) {
				for (auto& item : group.items) {
					DrawStrip(item.rowIndex);
				}
			}
		}

		DrawScale();

		m_thisFrameMouseDragging = ImGui::IsMouseDragging(ImGuiMouseButton_Left);

		DrawRows();

		if (!m_dragging)
		{
			/* Selection */
			bool lastFrameUsed = m_selectionBox.IsLastFrameUsed();
			m_selectionBox.OnUIRender();
			bool thisFrameUsing = m_selectionBox.IsUsing();

			/* Begin box select */
			if (!lastFrameUsed && thisFrameUsing) {
				if (!ImGui::IsKeyDown(ImGuiKey_LeftCtrl))
					m_selectedKeyframes.clear();
			}

			/* End box select */
			if (lastFrameUsed && !thisFrameUsing) {
				/* Merge box select result */
			}

			/* Cherry picking reset condition */
			/* Only click on nothing will clear the set*/
			if (ImGui::IsMouseClicked(ImGuiMouseButton_Left) && !m_mouseClickedOnItem && !ImGui::IsKeyDown(ImGuiKey_LeftCtrl)) 
				m_selectedKeyframes.clear();
		}

		if (m_dragging) {
			float deltaX = ImGui::GetMousePos().x - m_mouseXWhenStartDragging;
			for (auto& keyframe : m_selectedKeyframes) {
				keyframe->frame = m_keyframeFrameNumberMap[keyframe] + deltaX / COLUMN_WIDTH;
			}
		}

		if (m_dragging && ImGui::IsMouseReleased(ImGuiMouseButton_Left)) {
			MM_INFO("End dragging");
			m_dragging = false;
			m_keyframeFrameNumberMap.clear();
		}

		m_lastFrameMouseDragged = m_thisFrameMouseDragging;
		m_mouseClickedOnItem = false;

		ImGui::EndGroup();
		ImGui::End();
	}

	void Sequencer::OnModelLoaded(const EditorEvent::ModelLoaded& e)
	{
		m_model = e.model;

		if (m_model != nullptr) {
			const auto& pmx = m_model->GetPMXFile();
			const auto& pmxClusters = m_model->GetPMXFile().GetClusters();
			for (uint32_t clusterIndex = 0; clusterIndex < pmxClusters.size(); ++clusterIndex) {
				const auto& pc = pmxClusters[clusterIndex];
				Sequencer::Group group = {};
				group.name = pc.nameJP;
				for (uint32_t elemIndex = 0; elemIndex < pc.elements.size(); ++elemIndex) {
					const auto& elem = pc.elements[elemIndex];
					Sequencer::Item item = {};
					item.type = elem.type;
					item.index = elem.index;
					item.groupIndex = clusterIndex;
					switch (elem.type) {
					case PMXFile::CLUSTER_BONE:
						item.name = pmx.GetBoneName(elem.index);
						break;
					case PMXFile::CLUSTER_MORPH:
						item.name = pmx.GetMorphName(elem.index);
						break;
					}
					group.items.push_back(item);
				}
				AddGroup(std::move(group));
			}
		}
	}

	void Sequencer::OnMotionLoaded(const EditorEvent::MotionLoaded& e)
	{
		if (m_model != nullptr) {
			m_playing = false;
			m_frameCounter.Set(0);
			UpdateAnim();
		}
	}

	void Sequencer::OnUpdate(float deltaTime)
	{
		if (m_playing) {
			m_frameCounter.Step(deltaTime);
			UpdateAnim();
		}
	}

	void Sequencer::UpdateAnim()
	{
		if (m_model != nullptr) {
			m_model->GetAnim().Update(m_frameCounter.GetFrame(), m_frameCounter.GetSubFrame());
		}
	}
}

