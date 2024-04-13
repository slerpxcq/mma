#pragma once

#include <dexode/EventBus.hpp>
#include "Core/App/Event.hpp"

#include "Core/App/Application.hpp"

namespace mm
{
	class KeyframeEditor;
	class Animation;
	class Model;

	class Sequencer
	{
		static constexpr ImVec2 BUTTON_SIZE = ImVec2(10, 10);
		static constexpr uint32_t BUTTON_COLOR = 0x7fc0c0c0;
		static constexpr uint32_t ROW_HEIGHT = 20;
		static constexpr uint32_t BACKGROUND_COLOR = 0xff242424;
		static constexpr uint32_t HEADER_COLOR = 0xff3d3837;
		static constexpr uint32_t LEGEND_LENGTH = 150;
		static constexpr uint32_t COLUMN_WIDTH = 20;
		static constexpr uint32_t RULER_LONG_MARK_MULTIPLIER = 5;
		static constexpr uint32_t RULER_LONG_MARK_LENGTH = 10;
		static constexpr uint32_t RULER_SHORT_MARK_LENGTH = 5;
		static constexpr uint32_t RULER_MARK_COLOR = 0xffc0c0c0;
		static constexpr uint32_t VERTICAL_MARK_COLOR = 0x20c0c0c0;
		static constexpr uint32_t RULER_SCALE = 20;
		static constexpr uint32_t RULER_TEXT_OFFSET = 5;
		static constexpr uint32_t LEGEND_TEXT_OFFSET = 10;
		static constexpr uint32_t STRIP_COLOR_EVEN = 0xff363636;
		static constexpr uint32_t STRIP_COLOR_ODD = 0xff413d3d;

		static constexpr float DOPE_RADIUS = 7.5f;
		static constexpr uint32_t DOPE_FILL_COLOR = 0xffc0c0c0;
		static constexpr uint32_t DOPE_OUTLINE_COLOR = 0xff101010;
		static constexpr float DOPE_OUTLINE_SIZE = 1.5f;

		static constexpr float POINT_RADIUS = 4.5f;
		static constexpr uint32_t POINT_FILL_COLOR = 0xff000000;
		static constexpr uint32_t POINT_OUTLINE_COLOR = 0xff0080ff;
		static constexpr float POINT_OUTLINE_SIZE = 3.f;
		static constexpr float Y_GAIN = 5.f;
		static constexpr uint32_t INDENT_BASE = 15;

		static constexpr uint32_t CURVE_EDITOR_ROW_COUNT = 16;

	public:
		struct Item {
			std::string name;
			int32_t type;
			int32_t index;
			bool expanded = false;
			// For drawing
			int32_t rowIndex;
		};

		struct Group {
			std::string name;
			bool expanded = false;
			std::vector<Item> items;
			// For drawing
			int32_t rowIndex;
		};

	public:
		Sequencer(KeyframeEditor& ke) : 
			m_keyframeEditor(ke),
			m_listener(Application::Instance().GetEventBus()) {
			m_listener.listen<Event::MouseScrolled>(MM_EVENT_FN(Sequencer::OnMouseScrolled));
			m_listener.listen<Event::MouseButtonPressed>(MM_EVENT_FN(Sequencer::OnMouseButtonPressed));
		}

		void OnUIRender();
		void AddGroup(const Group& group) { m_groups.push_back(group); }
		void SetModel(Model* model) { m_model = model; }

	private:
		void CurveEditor(Item& item);

		// Events
		void OnMouseScrolled(const Event::MouseScrolled& e);
		void OnMouseButtonPressed(const Event::MouseButtonPressed& e);

		// Drawing
		void DrawExpandButton(uint32_t rowIndex, float offsetX, bool& expanded);
		void DrawDiamond(const ImVec2& center, float radius, float outlineSize, uint32_t outlineColor, uint32_t fillColor);
		template<typename T>
		void DrawRow(T& row, bool expandable, float textOffset);
		template<typename T>
		void DrawDope(const Item& item, const std::vector<T>& keyframeList);

		const char* GetButtonId() {
			static char buf[16];
			std::snprintf(buf, sizeof(buf), "B_%u", m_buttonIndex++);
			return buf;
		}

	private:
		KeyframeEditor& m_keyframeEditor;
		Model* m_model = nullptr;
		std::vector<Group> m_groups;

		ImDrawList* m_drawList = nullptr;

		int32_t m_selectedRow = -1;
		int32_t m_selectedColumn = -1;

		bool m_hovered = false;

		// Drawing states
		int32_t m_rowStart = 1;
		int32_t m_rowCount = 0;
		int32_t m_buttonIndex = 0;
		ImVec2 m_min;
		ImVec2 m_max;
		ImVec2 m_origin;
		ImVec2 m_size;

		ImVec2 m_rectMin;
		ImVec2 m_rectMax;

		int32_t m_minFrame = 0;
		int32_t m_maxFrame = 0;

		dexode::EventBus::Listener m_listener;
	};
}

