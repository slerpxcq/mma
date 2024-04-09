#include "mmpch.hpp"
#include "Sequencer.hpp"

namespace mm
{
	RampEdit::RampEdit()
   {
	  mPts[0][0] = ImVec2(-10.f, 0);
	  mPts[0][1] = ImVec2(20.f, 0.6f);
	  mPts[0][2] = ImVec2(25.f, 0.2f);
	  mPts[0][3] = ImVec2(70.f, 0.4f);
	  mPts[0][4] = ImVec2(120.f, 1.f);
	  mPointCount[0] = 5;

	  mPts[1][0] = ImVec2(-50.f, 0.2f);
	  mPts[1][1] = ImVec2(33.f, 0.7f);
	  mPts[1][2] = ImVec2(80.f, 0.2f);
	  mPts[1][3] = ImVec2(82.f, 0.8f);
	  mPointCount[1] = 4;


	  mPts[2][0] = ImVec2(40.f, 0);
	  mPts[2][1] = ImVec2(60.f, 0.1f);
	  mPts[2][2] = ImVec2(90.f, 0.82f);
	  mPts[2][3] = ImVec2(150.f, 0.24f);
	  mPts[2][4] = ImVec2(200.f, 0.34f);
	  mPts[2][5] = ImVec2(250.f, 0.12f);
	  mPointCount[2] = 6;
	  mbVisible[0] = mbVisible[1] = mbVisible[2] = true;
	  mMax = ImVec2(1.f, 1.f);
	  mMin = ImVec2(0.f, 0.f);
   }

	int RampEdit::EditPoint(size_t curveIndex, int pointIndex, ImVec2 value)
   {
	  mPts[curveIndex][pointIndex] = ImVec2(value.x, value.y);
	  SortValues(curveIndex);
	  for (size_t i = 0; i < GetPointCount(curveIndex); i++)
	  {
		 if (mPts[curveIndex][i].x == value.x)
			return (int)i;
	  }
	  return pointIndex;
   }

   void RampEdit::AddPoint(size_t curveIndex, ImVec2 value)
   {
	  if (mPointCount[curveIndex] >= 8)
		 return;
	  mPts[curveIndex][mPointCount[curveIndex]++] = value;
	  SortValues(curveIndex);
   }

	void KeyframeSequence::Get(int index, int** start, int** end, int* type, unsigned int* color)
	{
	  Item& item = items[index];
	  if (color)
		 *color = 0xFFAA8080; // same color for everyone, return color based on type
	  if (start)
		 *start = &item.startFrame;
	  if (end)
		 *end = &item.endFrame;
	  if (type)
		 *type = 0;
	}

	void KeyframeSequence::DoubleClick(int index)
	{
	  if (items[index].expanded)
	  {
		 items[index].expanded = false;
		 return;
	  }
	  for (auto& item : items)
		 item.expanded = false;
	  items[index].expanded = !items[index].expanded;
	}

	void KeyframeSequence::CustomDraw(int index, ImDrawList* draw_list, const ImRect& rc, const ImRect& legendRect, const ImRect& clippingRect, const ImRect& legendClippingRect)
	{
		  static const char* labels[] = { "Translation", "Rotation" , "Scale" };

		  rampEdit.mMax = ImVec2(float(maxFrame), 1.f);
		  rampEdit.mMin = ImVec2(float(minFrame), 0.f);
		  draw_list->PushClipRect(legendClippingRect.Min, legendClippingRect.Max, true);
		  for (int i = 0; i < 3; i++)
		  {
			 ImVec2 pta(legendRect.Min.x + 30, legendRect.Min.y + i * 14.f);
			 ImVec2 ptb(legendRect.Max.x, legendRect.Min.y + (i + 1) * 14.f);
			 draw_list->AddText(pta, rampEdit.mbVisible[i] ? 0xFFFFFFFF : 0x80FFFFFF, labels[i]);
			 if (ImRect(pta, ptb).Contains(ImGui::GetMousePos()) && ImGui::IsMouseClicked(0))
				rampEdit.mbVisible[i] = !rampEdit.mbVisible[i];
		  }
		  draw_list->PopClipRect();

		  ImGui::SetCursorScreenPos(rc.Min);
		  ImCurveEdit::Edit(rampEdit, rc.Max - rc.Min, 137 + index, &clippingRect);
	}

	void KeyframeSequence::CustomDrawCompact(int index, ImDrawList* draw_list, const ImRect& rc, const ImRect& clippingRect)
	{
		  rampEdit.mMax = ImVec2(float(maxFrame), 1.f);
		  rampEdit.mMin = ImVec2(float(minFrame), 0.f);
		  draw_list->PushClipRect(clippingRect.Min, clippingRect.Max, true);
		  for (int i = 0; i < 3; i++)
		  {
			 for (int j = 0; j < rampEdit.mPointCount[i]; j++)
			 {
				float p = rampEdit.mPts[i][j].x;
				if (p < items[index].startFrame || p > items[index].endFrame)
				   continue;
				float r = (p - minFrame) / float(maxFrame - minFrame);
				float x = ImLerp(rc.Min.x, rc.Max.x, r);
				draw_list->AddLine(ImVec2(x, rc.Min.y + 6), ImVec2(x, rc.Max.y - 4), 0xAA000000, 4.f);
			 }
		  }
		  draw_list->PopClipRect();
	}

	Sequencer::Sequencer()
	{
		m_sequence.minFrame = -100;
		m_sequence.maxFrame = 1000;
		//m_sequence.items.push_back(KeyframeSequence::Item{ "item", 10, 30, false });
		//m_sequence.items.push_back(KeyframeSequence::Item{ "item", 20, 30, true });
		//m_sequence.items.push_back(KeyframeSequence::Item{ "item", 12, 60, false });
		//m_sequence.items.push_back(KeyframeSequence::Item{ "item", 61, 90, false });
		//m_sequence.items.push_back(KeyframeSequence::Item{ "item", 90, 99, false });
	}

	void Sequencer::OnUIRender()
	{
		ImGui::Begin("Sequencer");
		static int selectedEntry = -1;
		static int firstFrame = 0;
		static bool expanded = true;
		static int currentFrame = 100;

		ImGui::PushItemWidth(130);
		ImGui::InputInt("Frame Min", &m_sequence.minFrame);
		ImGui::SameLine();
		ImGui::InputInt("Frame ", &currentFrame);
		ImGui::SameLine();
		ImGui::InputInt("Frame Max", &m_sequence.maxFrame);
		ImGui::PopItemWidth();
		ImSequencer::Sequencer(
			&m_sequence, 
			&currentFrame, 
			&expanded, 
			&selectedEntry, 
			&firstFrame, 
			ImSequencer::SEQUENCER_EDIT_STARTEND | 
			ImSequencer::SEQUENCER_ADD | 
			ImSequencer::SEQUENCER_DEL | 
			ImSequencer::SEQUENCER_COPYPASTE | 
			ImSequencer::SEQUENCER_CHANGE_FRAME);

		ImGui::End();
	}
}
