#pragma once

#include <ImSequencer.h>
#include <ImCurveEdit.h>

#include <imgui_internal.h>
#include "Core/Utility/Type.hpp"

static const char* SequencerItemTypeNames[] = { "Camera","Music", "ScreenEffect", "FadeIn", "Animation" };

namespace mm
{
	struct RampEdit : public ImCurveEdit::Delegate
	{
		RampEdit();

	   size_t GetCurveCount() { return 3; }

	   bool IsVisible(size_t curveIndex) { return mbVisible[curveIndex]; }
	   size_t GetPointCount(size_t curveIndex) { return mPointCount[curveIndex]; }

	   uint32_t GetCurveColor(size_t curveIndex) {
		  uint32_t cols[] = { 0xFF0000FF, 0xFF00FF00, 0xFFFF0000 };
		  return cols[curveIndex];
	   }

	   ImVec2* GetPoints(size_t curveIndex) { return mPts[curveIndex]; }
	   virtual ImCurveEdit::CurveType GetCurveType(size_t curveIndex) const { return ImCurveEdit::CurveSmooth; }
	   virtual int EditPoint(size_t curveIndex, int pointIndex, ImVec2 value);
	   virtual void AddPoint(size_t curveIndex, ImVec2 value);
	   virtual ImVec2& GetMax() { return mMax; }
	   virtual ImVec2& GetMin() { return mMin; }
	   virtual unsigned int GetBackgroundColor() { return 0; }

	public:
	   ImVec2 mPts[3][8];
	   size_t mPointCount[3];
	   bool mbVisible[3];
	   ImVec2 mMin;
	   ImVec2 mMax;

	private:
	   void SortValues(size_t curveIndex) {
		  auto b = std::begin(mPts[curveIndex]);
		  auto e = std::begin(mPts[curveIndex]) + GetPointCount(curveIndex);
		  std::sort(b, e, [](ImVec2 a, ImVec2 b) { return a.x < b.x; });
	   }
	};

	struct KeyframeSequence : public ImSequencer::SequenceInterface
	{
	public:
	   KeyframeSequence() : minFrame(0), maxFrame(0) {}

	   virtual int GetFrameMin() const { return minFrame; }
	   virtual int GetFrameMax() const { return maxFrame; }
	   virtual int GetItemCount() const { return (int)items.size(); }

	   virtual int GetItemTypeCount() const { return 0; }
	   virtual const char* GetItemTypeName(int typeIndex) const { return ""; }
	   virtual const char* GetItemLabel(int index) const { return items[index].name.c_str(); }

	   virtual void Get(int index, int** start, int** end, int* type, unsigned int* color);
	   virtual void Add(int type) {}
	   virtual void Del(int index) {}
	   virtual void Duplicate(int index) {}
	   virtual size_t GetCustomHeight(int index) { return items[index].expanded ? 300 : 0; }

	   virtual void DoubleClick(int index);
	   virtual void CustomDraw(int index, ImDrawList* draw_list, const ImRect& rc, const ImRect& legendRect, const ImRect& clippingRect, const ImRect& legendClippingRect);
	   virtual void CustomDrawCompact(int index, ImDrawList* draw_list, const ImRect& rc, const ImRect& clippingRect);

	public:
	   struct Item {
		   std::string name;
		  int startFrame, endFrame;
		  bool expanded;
	   };

	public:
	   int minFrame, maxFrame;
	   std::vector<Item> items;
	   RampEdit rampEdit;
	};

	class Sequencer
	{
	public:
		Sequencer();
		void AddItem(const KeyframeSequence::Item& item) {
			m_sequence.items.push_back(item);
		}

		void OnUIRender();

	private:
		KeyframeSequence m_sequence;
	};
}
