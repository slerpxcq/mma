#include "mmpch.hpp"
#include "Sequencer.hpp"

#include "Core/MM/Model/Model.hpp"

#include "Core/Utility/Type.hpp"
#include "Core/App/Input.hpp"

#include "EditorLayer.hpp"
#include "Properties.hpp"

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
		ImVec2 textPos = ImVec2(LEGEND_TEXT_OFFSET + textOffset, y);
		m_drawList->AddText(m_canvasOrigin + textPos, RULER_MARK_COLOR, row.name.c_str());

		/* Button only for Item */
		if constexpr (std::is_same_v<T, Item>) {
			if (row.type == PMXFile::CLUSTER_BONE) {
				ImGui::SetCursorScreenPos(m_canvasOrigin + textPos);
				if (ImGui::InvisibleButton(GenButtonId(), ImVec2(LEGEND_LENGTH - textPos.x, ROW_HEIGHT))) {
					m_editor.m_curveEditor->SetContainer(m_model->GetAnim().GetBoneKeyframeMatrix()[row.index]);
				}
			}
		}

		if (expandable) 
			DrawExpandButton(row.rowIndex, textOffset - 5, row.expanded);
	}

	//template <typename T>
	//typename Animation::KeyframeContainer<T>::iterator Sequencer::FirstKeyframeOnCanvas(Animation::KeyframeContainer<T>& keyframeList)
	//{
	//	auto it = std::lower_bound(
	//		keyframeList.begin(), 
	//		keyframeList.end(), 
	//		m_minFrame, 
	//		[](const T& lhs, uint32_t rhs) {
	//			return lhs.frame < rhs;
	//		});
	//	return it;
	//}

	bool Sequencer::IsKeyframeSelected(Animation::Keyframe* keyframe)
	{
		auto it = std::find_if(m_selectedDopes.begin(), m_selectedDopes.end(),
			[keyframe](const std::shared_ptr<DopeBase>& dp) {
				return dp->keyframe == keyframe;
			});
		return it != m_selectedDopes.end();
	}

	void Sequencer::OnItemSelected(const EditorEvent::ItemSelected& e)
	{
		if (e.type == Properties::TYPE_MODEL) {
			SetModel(std::any_cast<Model*>(e.item));
		}
	}

	void Sequencer::DrawGroupDope(const Group& group)
	{
	}

	template <typename T>
	void Sequencer::DrawItemDope(const Item& item, Animation::KeyframeContainer<T>& keyframeList)
	{
		/* The first row is the ruler, don't draw on it! */
		if (item.rowIndex < 1)
			return;

		auto keyframe = LowerBound(keyframeList, m_minFrame);

		for (; keyframe != keyframeList.end() && keyframe->frame < m_maxFrame; ++keyframe) {
			float x = LEGEND_LENGTH + ROW_HEIGHT / 2;
			float y = item.rowIndex * ROW_HEIGHT;
			uint32_t column = keyframe->frame - m_minFrame;

			ImVec2 dopePos = ImVec2(x + column * COLUMN_WIDTH, y + ROW_HEIGHT / 2);
			ImVec2 buttonSize = 2.f * ImVec2(DOPE_RADIUS, DOPE_RADIUS);
			ImVec2 buttonPos = dopePos - 0.5f * buttonSize;

			ImGui::SetCursorScreenPos(m_canvasOrigin + buttonPos);
			if (ImGui::InvisibleButton(GenButtonId(), buttonSize)) {
				m_thisFrameAnyDopeClicked = true;
				if (!IsKeyframeSelected(&(*keyframe)))
					m_selectedDopes.emplace(std::make_shared<Dope<T>>(keyframeList, &(*keyframe)));
			}
			if (ImGui::IsItemHovered()) {
				m_thisFrameAnyDopeHovered = true;
			}
			if (m_state == State::BOX_SELECT && m_selectionBox.CheckIntersection(m_canvasOrigin + buttonPos, DOPE_RADIUS)) {
				if (!IsKeyframeSelected(&(*keyframe)))
					m_selectedDopes.emplace(std::make_shared<Dope<T>>(keyframeList, &(*keyframe)));
			}

			/* Draw */
			if (IsKeyframeSelected(&(*keyframe))) {
				DrawDiamond(dopePos, DOPE_RADIUS, DOPE_OUTLINE_SIZE_SELECTED, DOPE_OUTLINE_COLOR_SELECTED, DOPE_FILL_COLOR);
			}
			else {
				DrawDiamond(dopePos, DOPE_RADIUS, DOPE_OUTLINE_SIZE, DOPE_OUTLINE_COLOR, DOPE_FILL_COLOR);
			}
		}
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

		/* Ruler background */
		m_drawList->AddRectFilled(m_canvasOrigin, ImVec2(m_canvasOrigin.x + m_canvasSize.x, m_canvasOrigin.y + ROW_HEIGHT), HEADER_COLOR);

		for (uint32_t rulerX = rulerBeginX; rulerX <= rulerEndX; rulerX += COLUMN_WIDTH, ++column, ++columnCount) {
			float lineBeginY = ROW_HEIGHT;
			float lineEndY = ROW_HEIGHT * m_visibleRowCount;

			ImVec2 buttonPos = ImVec2(rulerX - COLUMN_WIDTH / 2, 0);
			ImVec2 buttonSize = ImVec2(COLUMN_WIDTH, ROW_HEIGHT);

			ImGui::SetCursorScreenPos(m_canvasOrigin + buttonPos);
			if (ImGui::InvisibleButton(GenButtonId(), buttonSize)) {
				EventBus::Instance()->postpone<EditorEvent::FrameSet>({ column });
				m_selectedFrame = column;
				SetFrame(m_selectedFrame);
			}

			/* Draw current frame mark (red vertical line) */
			if (column == m_selectedFrame) {
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

	std::unique_ptr<SequencerClipboardContent> Sequencer::MakeClipboardContentFromSelected()
	{
		auto content = std::make_unique<SequencerClipboardContent>();
		for (auto& dope : m_selectedDopes) 
			content->dopes.push_back(dope->Clone());
		return content;
	}

	void Sequencer::ProcessKeys()
	{
		/* Copy */
		if (ImGui::IsKeyPressed(ImGuiKey_C) && ImGui::IsKeyDown(ImGuiKey_LeftCtrl)) {
			if (!m_selectedDopes.empty())
				Clipboard::Instance().SetContent(MakeClipboardContentFromSelected());
		}

		/* Paste */
		if (ImGui::IsKeyPressed(ImGuiKey_V) && ImGui::IsKeyDown(ImGuiKey_LeftCtrl)) {
			SequencerClipboardContent* content = dynamic_cast<SequencerClipboardContent*>(Clipboard::Instance().GetContent());
			if (content != nullptr) {
				auto min = std::min_element(content->dopes.begin(), content->dopes.end(), 
					[](const std::shared_ptr<DopeBase>& lhs, const std::shared_ptr<DopeBase>& rhs){
						return lhs->keyframe->frame < rhs->keyframe->frame;
					});
				if (min != content->dopes.end()) {
					uint32_t minFrame = (*min)->keyframe->frame;
					for (auto& dope : content->dopes) 
						dope->Duplicate(m_selectedFrame + dope->keyframe->frame - minFrame);
				}
			}
			UpdateAnim();
		}

		/* Cut */
		if (ImGui::IsKeyPressed(ImGuiKey_X) && ImGui::IsKeyDown(ImGuiKey_LeftCtrl)) {
			if (!m_selectedDopes.empty()) {
				Clipboard::Instance().SetContent(MakeClipboardContentFromSelected());
				for (auto& dope : m_selectedDopes) {
					if (dope->keyframe->frame == 0)
						continue;
					dope->Delete();
				}
				m_selectedDopes.clear();
				UpdateAnim();
			}
		}

		/* Delete */
		if (ImGui::IsKeyPressed(ImGuiKey_Delete)) {
			if (!m_selectedDopes.empty()) {
				EventBus::Instance()->postpone<EditorEvent::CommandIssued>({ new SequencerDopeDeletedCommand(m_selectedDopes) });
				for (auto& dope : m_selectedDopes) {
					if (dope->keyframe->frame == 0)
						continue;
					dope->Delete();
				}
				m_selectedDopes.clear();
				UpdateAnim();
			}
		}

		if (ImGui::IsKeyPressed(ImGuiKey_RightArrow)) {
			++m_selectedFrame;
			SetFrame(m_selectedFrame);
		}

		if (ImGui::IsKeyPressed(ImGuiKey_LeftArrow)) {
			if (m_selectedFrame == 0)
				return;
			--m_selectedFrame;
			SetFrame(m_selectedFrame);
		}
	}

	void Sequencer::CheckAutoScroll(uint32_t frame)
	{
		if (frame - m_minFrame <= AUTO_SCROLL_THRESHOLD) {
			int32_t newMinFrame = m_minFrame - AUTO_SCROLL_STEP;
			newMinFrame = std::max(0, newMinFrame);
			m_minFrame = newMinFrame;
		}

		if (m_maxFrame - frame <= AUTO_SCROLL_THRESHOLD) {
			m_minFrame += AUTO_SCROLL_STEP;
		}
	}

	void Sequencer::SetFrame(uint32_t frame)
	{
		m_frameCounter.Set(frame);
		UpdateAnim();
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
		if (ImGui::SliderInt("Min frame", &m_minFrame, 0, 10000)) {
		}
		ImGui::SameLine();
		if (ImGui::InputInt(" ", &m_minFrame)) {
		}
		m_minFrame = std::clamp(m_minFrame, 0, 10000);

		m_hovered = ImGui::IsWindowHovered();
		m_drawList = ImGui::GetWindowDrawList();
		m_canvasMin = ImGui::GetCursorPos();
		m_canvasMax = ImGui::GetWindowContentRegionMax();
		m_canvasOrigin = ImGui::GetWindowPos() + m_canvasMin;
		m_canvasSize = m_canvasMax - m_canvasMin;
		m_buttonIndex = 0;
		m_visibleRowCount = m_canvasSize.y / ROW_HEIGHT;
		m_thisFrameMouseDragged = ImGui::IsMouseDragging(ImGuiMouseButton_Left);

		ImGui::BeginGroup();

		/*************************************** BEGIN DRAWING ***************************************/
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
		DrawRows();
		/*************************************** END DRAWING ***************************************/

		bool beginDragging = !m_lastFrameMouseDragged && m_thisFrameMouseDragged;
		bool endDragging = m_lastFrameMouseDragged && !m_thisFrameMouseDragged;

		/* State transition */
		switch (m_state) {
		case State::IDLE:
			if (ImGui::IsMouseClicked(ImGuiMouseButton_Left) && !m_thisFrameAnyDopeClicked && !m_thisFrameAnyDopeHovered) {
				m_selectedDopes.clear();
			}
			if (ImGui::IsKeyPressed(ImGuiKey_LeftCtrl)) {
				m_state = State::CHERRY_PICK;
				break;
			}
			if (beginDragging && !m_thisFrameAnyDopeHovered) {
				//m_selectedKeyframes.clear();
				m_selectionBox.SetBegin(ImGui::GetMousePos());
				m_state = State::BOX_SELECT;
				break;
			}
			if (beginDragging && m_thisFrameAnyDopeHovered) {
				for (const auto& dope : m_selectedDopes) {
					m_keyframeFrameOnStartDragging[dope.get()] = dope->keyframe->frame;
				}
				m_state = State::DOPE_DRAG;
				break;
			}
			break;
		case State::CHERRY_PICK:
			if (ImGui::IsKeyReleased(ImGuiKey_LeftCtrl)) {
				m_state = State::IDLE;
				break;
			}
			break;
		case State::BOX_SELECT:
			m_selectionBox.OnUIRender();
			if (endDragging) {
				m_state = State::IDLE;
				break;
			}
			break;
		case State::DOPE_DRAG:
			{
				/* Process dragging */
				float deltaX = ImGui::GetMouseDragDelta().x;
				bool hasCollision = false;
				//for (auto& dope : m_selectedDopes) {
				//	uint32_t targetFrame = m_keyframeFrameOnStartDragging[dope.get()] + deltaX / COLUMN_WIDTH;
				//	/* Need to access the keyframe container */
				//	/* Need to sort the keyframe container; Use std::set instead? */
				//}
				for (auto& dope : m_selectedDopes) {
					/* Don't touch the first keyframe! */
					if (dope->keyframe->frame == 0)
						continue;
					/* Check if keyframe collision */
					dope->keyframe->frame = m_keyframeFrameOnStartDragging[dope.get()] + deltaX / COLUMN_WIDTH;
				}
			}
			if (endDragging) {
				/* Make command */
				std::vector<SequencerKeyframeDraggedCommand::UndoData> undoDatas;

				for (auto& dope : m_selectedDopes) 
					undoDatas.push_back({ dope->keyframe, m_keyframeFrameOnStartDragging[dope.get()] });

				EventBus::Instance()->postpone<EditorEvent::CommandIssued>({ new SequencerKeyframeDraggedCommand(std::move(undoDatas)) });

				m_state = State::IDLE;
			}
			break;
		}

		ImGui::EndGroup();

		ProcessKeys();
		CheckAutoScroll(m_selectedFrame);

		ImGui::End();

		m_lastFrameMouseDragged = m_thisFrameMouseDragged;
		m_thisFrameAnyDopeClicked = false;
		m_thisFrameAnyDopeHovered = false;
	}
	 
	void Sequencer::SetModel(Model* model)
	{
		m_groups.clear();
		m_model = model;

		/* Load all fields */
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

