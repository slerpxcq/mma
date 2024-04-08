#pragma once

#include <ImCurveEdit.h>
#include <ImSequencer.h>
#include <ImGui.h>
#include <imgui_internal.h>

namespace mm
{
	struct RampEdit : public ImCurveEdit::Delegate
	{
		RampEdit();
		size_t GetCurveCount() { return 3; }
		bool IsVisible(size_t curveIndex) { return mbVisible[curveIndex]; }
		size_t GetPointCount(size_t curveIndex)
		{
		   return mPointCount[curveIndex];
		}

		uint32_t GetCurveColor(size_t curveIndex)
		{
		   uint32_t cols[] = { 0xFF0000FF, 0xFF00FF00, 0xFFFF0000 };
		   return cols[curveIndex];
		}
		ImVec2* GetPoints(size_t curveIndex)
		{
		   return mPts[curveIndex];
		}
		virtual ImCurveEdit::CurveType GetCurveType(size_t curveIndex) const { return ImCurveEdit::CurveSmooth; }
		virtual int EditPoint(size_t curveIndex, int pointIndex, ImVec2 value)
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
		virtual void AddPoint(size_t curveIndex, ImVec2 value)
		{
		   if (mPointCount[curveIndex] >= 8)
			 return;
		   mPts[curveIndex][mPointCount[curveIndex]++] = value;
		   SortValues(curveIndex);
		}
		virtual ImVec2& GetMax() { return mMax; }
		virtual ImVec2& GetMin() { return mMin; }
		virtual unsigned int GetBackgroundColor() { return 0; }
		ImVec2 mPts[3][8];
		size_t mPointCount[3];
		bool mbVisible[3];
		ImVec2 mMin;
		ImVec2 mMax;
	private:
	   void SortValues(size_t curveIndex)
	   {
		  auto b = std::begin(mPts[curveIndex]);
		  auto e = std::begin(mPts[curveIndex]) + GetPointCount(curveIndex);
		  std::sort(b, e, [](ImVec2 a, ImVec2 b) { return a.x < b.x; });
	   }
	};

	struct MySequence : public ImSequencer::SequenceInterface
	{
	public:
	    MySequence() : minFrame(0), maxFrame(0) {}

		virtual int GetFrameMin() const { return minFrame; }
		virtual int GetFrameMax() const { return maxFrame; }
	    virtual int GetItemCount() const { return (int)items.size(); }

		virtual int GetItemTypeCount() const { return 0; }
	    virtual const char* GetItemTypeName(int typeIndex) const { return ""; }
		virtual const char* GetItemLabel(int index) const { return items[index].label.c_str(); }

	   virtual void Get(int index, int** start, int** end, int* type, unsigned int* color) {
		  MySequenceItem& item = items[index];
		  if (color)
			 *color = 0xFFAA8080; // same color for everyone, return color based on type
		  if (start)
			 *start = &item.mFrameStart;
		  if (end)
			 *end = &item.mFrameEnd;
		  if (type)
			 *type = item.mType;
	   }

	   virtual void Add(int type) {}
	   virtual void Del(int index) {}
	   virtual void Duplicate(int index) {}
	   virtual size_t GetCustomHeight(int index) { return 0; }

	   virtual void DoubleClick(int index) {
		  if (items[index].mExpanded)
		  {
			 items[index].mExpanded = false;
			 return;
		  }
		  for (auto& item : items)
			 item.mExpanded = false;
		  items[index].mExpanded = !items[index].mExpanded;
	   }

	   virtual void CustomDraw(int index, ImDrawList* draw_list, const ImRect& rc, const ImRect& legendRect, const ImRect& clippingRect, const ImRect& legendClippingRect)
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
		  ImCurveEdit::Edit(rampEdit, ImVec2(rc.Max.x - rc.Min.x, rc.Max.y-rc.Min.y), 137 + index, &clippingRect);
	   }

	   virtual void CustomDrawCompact(int index, ImDrawList* draw_list, const ImRect& rc, const ImRect& clippingRect)
	   {
		  rampEdit.mMax = ImVec2(float(maxFrame), 1.f);
		  rampEdit.mMin = ImVec2(float(minFrame), 0.f);
		  draw_list->PushClipRect(clippingRect.Min, clippingRect.Max, true);
		  for (int i = 0; i < 3; i++)
		  {
			 for (int j = 0; j < rampEdit.mPointCount[i]; j++)
			 {
				float p = rampEdit.mPts[i][j].x;
				if (p < items[index].mFrameStart || p > items[index].mFrameEnd)
				   continue;
				float r = (p - minFrame) / float(maxFrame - minFrame);
				float x = ImLerp(rc.Min.x, rc.Max.x, r);
				draw_list->AddLine(ImVec2(x, rc.Min.y + 6), ImVec2(x, rc.Max.y - 4), 0xAA000000, 4.f);
			 }
		  }
		  draw_list->PopClipRect();
	   }

	public:
	   struct MySequenceItem {
		   std::string label;
		   int mType;
		   int mFrameStart, mFrameEnd;
		   bool mExpanded;
	   };

	public:
	   // my datas
		std::string name;
	   int minFrame, maxFrame;
	   std::vector<MySequenceItem> items;
	   RampEdit rampEdit;
	};
}
;

