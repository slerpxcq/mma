#pragma once

namespace mm
{
	class SelectionBox
	{
		static constexpr uint32_t FILL_COLOR = IM_COL32(0, 130, 216, 255);
		static constexpr uint32_t OUTLINE_COLOR = IM_COL32(0, 130, 216, 50);

	public:
		void OnUIRender();
		bool CheckIntersection(const ImVec2& center, float radius);
		bool IsLastFrameUsed() { return m_lastFrameUsed; }
		bool IsThisFrameUsed() { return m_thisFrameUsed; }

	private:
		ImDrawList* m_drawList;
		ImVec2 m_p1;
		ImVec2 m_p2;
		ImVec2 m_min;
		ImVec2 m_max;
		bool m_lastFrameUsed = false;
		bool m_thisFrameUsed = false;
	};
}

