#include "mmpch.hpp"
#include "Sequencer.hpp"

#include "KeyframeEditor.hpp"
#include "Core/MM/Model/Model.hpp"

#include "Core/Utility/Type.hpp"

#include "Core/App/Application.hpp"
#include "Core/App/Layer/ImGuiLayer.hpp"

namespace mm
{
	void Sequencer::ExpandButton(uint32_t rowIndex, float offsetX, bool& expanded)
	{
		ImGuiIO& io = ImGui::GetIO();
		ImDrawList* drawList = ImGui::GetWindowDrawList();

		ImVec2 pos = ImVec2(offsetX, rowIndex * ROW_HEIGHT + 8);

		ImGui::SetCursorScreenPos(m_origin + pos);
		if (ImGui::InvisibleButton(GetButtonId(), BUTTON_SIZE)) {
			expanded = !expanded;
		}

		ImVec2 max = pos + BUTTON_SIZE;
		ImVec2 mid = pos + 0.5f * BUTTON_SIZE;
		if (!expanded) {
			// Lower triangle
			drawList->AddTriangleFilled(
				m_origin + pos, 
				m_origin + mid, 
				m_origin + ImVec2(max.x, pos.y), BUTTON_COLOR);
		}
		else {
			// Upper triangle
			drawList->AddTriangleFilled(
				m_origin + ImVec2(pos.x, mid.y),
				m_origin + ImVec2(mid.x, pos.y),
				m_origin + ImVec2(max.x, mid.y),
				BUTTON_COLOR);
		}
	}

	template <typename T>
	void Sequencer::DrawRow(T& row, bool expandable, float textOffset)
	{
		float y = row.rowIndex * ROW_HEIGHT;
		m_drawList->AddText(m_origin + ImVec2(LEGEND_TEXT_OFFSET + textOffset, y), RULER_MARK_COLOR, row.name.c_str());
		if (expandable) 
			ExpandButton(row.rowIndex, textOffset - 5, row.expanded);
		// Strip
		uint32_t stripColor = (row.rowIndex & 1) ? STRIP_COLOR_ODD : STRIP_COLOR_EVEN;
		m_drawList->AddRectFilled(
			m_origin + ImVec2(LEGEND_LENGTH, y),
			m_origin + ImVec2(m_size.x, y + ROW_HEIGHT),
			stripColor);
	}

	template <typename T>
	bool Sequencer::DrawDope(const Item& item, const std::vector<T>& keyframeList)
	{
		bool ret = false;

		auto it = std::lower_bound(
			keyframeList.begin(), 
			keyframeList.end(), 
			m_minFrame, 
			[](const T& lhs, uint32_t rhs) {
				return lhs.frame < rhs;
			});

		for (; it != keyframeList.end() && it->frame < m_maxFrame; ++it) {
			float startX = LEGEND_LENGTH + ROW_HEIGHT / 2;
			float y = item.rowIndex * ROW_HEIGHT;
			uint32_t column = it->frame - m_minFrame;
			ImVec2 dopePos = ImVec2(startX + column * COLUMN_WIDTH, y + ROW_HEIGHT / 2);
			ImVec2 p0 = m_origin + ImVec2(dopePos.x - DOPE_RADIUS, dopePos.y);
			ImVec2 p1 = m_origin + ImVec2(dopePos.x, dopePos.y - DOPE_RADIUS);
			ImVec2 p2 = m_origin + ImVec2(dopePos.x + DOPE_RADIUS, dopePos.y);
			ImVec2 p3 = m_origin + ImVec2(dopePos.x, dopePos.y + DOPE_RADIUS);
			m_drawList->AddQuad(p0, p1, p2, p3, DOPE_OUTLINE_COLOR, DOPE_OUTLINE_SIZE);
			m_drawList->AddQuadFilled(p0, p1, p2, p3, DOPE_FILL_COLOR);
			ImVec2 buttonSize = 2.f * ImVec2(DOPE_RADIUS, DOPE_RADIUS);
			ImVec2 buttonPos = dopePos - 0.5f * buttonSize;
			ImGui::SetCursorScreenPos(m_origin + buttonPos);
			if (ImGui::InvisibleButton(GetButtonId(), buttonSize)) {
				ret = true;
			}
		}

		return ret;
	}

	//void Sequencer::DrawCurve()
	//{

	//}

	void Sequencer::OnUIRender()
	{
		ImGui::Begin("Sequencer", nullptr,
			ImGuiWindowFlags_NoScrollbar |
			ImGuiWindowFlags_NoScrollWithMouse);
		if (ImGui::SliderInt("Min", &m_minFrame, 0, 10000)) {
			m_minFrame = std::clamp(m_minFrame, 0, 10000);
		}

		m_drawList = ImGui::GetWindowDrawList();

		// Init states
		m_min = ImGui::GetCursorPos();
		m_max = ImGui::GetWindowContentRegionMax();
		m_origin = ImGui::GetWindowPos() + m_min;
		m_size = m_max - m_min;
		m_rowCount = 0;
		m_buttonIndex = 0;

		ImGui::BeginGroup();

		// Background
		m_drawList->AddRectFilled(m_origin, m_origin + m_size, BACKGROUND_COLOR);
		m_drawList->AddRectFilled(m_origin, ImVec2(m_origin.x + m_size.x, m_origin.y + ROW_HEIGHT), HEADER_COLOR);

		// Get row index of each entry
		uint32_t rowIndex = 1;
		for (auto& group : m_groups) {
			group.rowIndex = rowIndex++;
			if (group.expanded) {
				for (auto& item : group.items) {
					item.rowIndex = rowIndex++;
					if (item.expanded) {
						rowIndex += CURVE_EDITOR_ROW_COUNT;
					}
				}
			}
		}
		m_rowCount = rowIndex - 1;

		// Draw groups
		for (auto& group : m_groups) {
			DrawRow(group, true, 10);
		}

		// Draw items
		for (auto& group : m_groups) {
			if (group.expanded) {
				for (auto& item : group.items) {
					switch (item.type) {
					case PMXFile::CLUSTER_BONE:
						DrawRow(item, true, 20);
						break;
					case PMXFile::CLUSTER_MORPH:
						DrawRow(item, false, 20);
						break;
					}
				}
			}
		}

		// Ruler
		float rulerBeginX = LEGEND_LENGTH + ROW_HEIGHT / 2;
		float rulerEndX = m_size.x;
		uint32_t column = m_minFrame;
		uint32_t columnCount = 0;
		for (uint32_t rulerX = rulerBeginX; rulerX <= rulerEndX; rulerX += COLUMN_WIDTH, ++column, ++columnCount) {
			float lineBeginY = ROW_HEIGHT;
			float lineEndY = lineBeginY + ROW_HEIGHT * m_rowCount;
			// Button for selecting current frame
			ImVec2 buttonPos = ImVec2(rulerX - COLUMN_WIDTH / 2, 0);
			ImVec2 buttonSize = ImVec2(COLUMN_WIDTH, ROW_HEIGHT);
			ImGui::SetCursorScreenPos(m_origin + buttonPos);
			if (ImGui::InvisibleButton(GetButtonId(), buttonSize)) {
				m_selectedColumn = column;
			}

			// Draw current frame mark
			if (column == m_selectedColumn) {
				m_drawList->AddLine(
					m_origin + ImVec2(rulerX, lineBeginY),
					m_origin + ImVec2(rulerX, lineEndY),
					IM_COL32(255, 0, 0, 64), ROW_HEIGHT / 2);
			}

			// Long mark
			if (column % RULER_LONG_MARK_MULTIPLIER == 0) {
				m_drawList->AddLine(
					m_origin + ImVec2(rulerX, ROW_HEIGHT),
					m_origin + ImVec2(rulerX, ROW_HEIGHT - RULER_LONG_MARK_LENGTH),
					RULER_MARK_COLOR);
				char buf[16];
				std::snprintf(buf, sizeof(buf), "%i", column);
				m_drawList->AddText(m_origin + ImVec2(rulerX + RULER_TEXT_OFFSET, 0), RULER_MARK_COLOR, buf);
				m_drawList->AddLine(
					m_origin + ImVec2(rulerX, lineBeginY),
					m_origin + ImVec2(rulerX, lineEndY),
					VERTICAL_MARK_COLOR, 2.0f);
			}
			// Short mark
			else {
				m_drawList->AddLine(
					m_origin + ImVec2(rulerX, ROW_HEIGHT),
					m_origin + ImVec2(rulerX, ROW_HEIGHT - RULER_SHORT_MARK_LENGTH),
					RULER_MARK_COLOR);
				m_drawList->AddLine(
					m_origin + ImVec2(rulerX, lineBeginY),
					m_origin + ImVec2(rulerX, lineEndY),
					VERTICAL_MARK_COLOR);
			}
		}
		m_maxFrame = m_minFrame + columnCount;

		// Dope and curve editor
		if (m_model != nullptr) {
			Animation* anim = m_model->GetAnim();
			if (anim != nullptr) {
				for (auto& group : m_groups) {
					if (group.expanded) {
						for (auto& item : group.items) {
							switch (item.type) {
							case PMXFile::CLUSTER_BONE:
								DrawDope(item, anim->GetBoneKeyframes()[item.index]);
								break;
							case PMXFile::CLUSTER_MORPH:
								DrawDope(item, anim->GetMorphKeyframes()[item.index]);
								break;
							default:
								break;
							}
							if (item.expanded) {
								// Curve editor
								ImVec2 editorOrigin = ImVec2(LEGEND_LENGTH, (item.rowIndex + 1) * ROW_HEIGHT);
								float editorHeight = CURVE_EDITOR_ROW_COUNT * ROW_HEIGHT;
								float midY = editorOrigin.y + editorHeight * 0.5f;
								// NOTE: only bone keyframes are expandable.
								MM_ASSERT(item.type == PMXFile::CLUSTER_BONE);
								auto& keyframeList = anim->GetBoneKeyframes()[item.index];
								auto it = std::lower_bound(
									keyframeList.begin(),
									keyframeList.end(),
									m_minFrame,
									[](const Animation::Keyframe& lhs, uint32_t rhs) {
										return lhs.frame < rhs;
									});

								for (; it != keyframeList.end() && it->frame < m_maxFrame; ++it) {
									// for all axes
									static constexpr uint32_t axisColor[] = {
										IM_COL32(255, 0, 0, 128), // X->R
										IM_COL32(0, 255, 0, 128), // Y->G
										IM_COL32(0, 0, 255, 128)}; // Z->B

									for (uint32_t axis = 0; axis < 3; ++axis) {
										float y = midY + it->xform.trans[axis] * Y_GAIN;
										float startX = LEGEND_LENGTH + ROW_HEIGHT / 2;
										uint32_t column = it->frame - m_minFrame;
										ImVec2 diamondPos = ImVec2(startX + column * COLUMN_WIDTH, y + ROW_HEIGHT / 2);
										ImVec2 p0 = m_origin + ImVec2(diamondPos.x - POINT_RADIUS, diamondPos.y);
										ImVec2 p1 = m_origin + ImVec2(diamondPos.x, diamondPos.y - POINT_RADIUS);
										ImVec2 p2 = m_origin + ImVec2(diamondPos.x + POINT_RADIUS, diamondPos.y);
										ImVec2 p3 = m_origin + ImVec2(diamondPos.x, diamondPos.y + POINT_RADIUS);
										m_drawList->AddQuad(p0, p1, p2, p3, POINT_OUTLINE_COLOR, POINT_OUTLINE_SIZE);
										m_drawList->AddQuadFilled(p0, p1, p2, p3, POINT_FILL_COLOR);
										if (it + 1 != keyframeList.end()) {
											ImVec2 bezP0 = diamondPos;
											float nextY = midY + (it + 1)->xform.trans[axis] * Y_GAIN;
											uint32_t nextColumn = (it + 1)->frame - m_minFrame;
											ImVec2 bezP3 = ImVec2(startX + nextColumn * COLUMN_WIDTH, nextY + ROW_HEIGHT / 2);
											ImVec2 delta = bezP3 - bezP0;
											ImVec2 vmdP1 = ImVec2(it->bez.GetHandles()[axis][0].x, it->bez.GetHandles()[axis][0].y);
											ImVec2 bezP1 = ((vmdP1 * (1.f / 127)) * delta) + bezP0;
											ImVec2 vmdP2 = ImVec2(it->bez.GetHandles()[axis][1].x, it->bez.GetHandles()[axis][1].y);
											ImVec2 bezP2 = ((vmdP2 * (1.f / 127)) * delta) + bezP0;
											m_drawList->AddBezierCubic(
												m_origin + bezP0, 
												m_origin + bezP1, 
												m_origin + bezP2, 
												m_origin + bezP3,
												axisColor[axis], 3.0f);
										}
									}
								}
							}
						}
					}
				}
			}
		}

		ImGui::EndGroup();
		ImGui::End();
	}
}
