#pragma once

#include <dexode/EventBus.hpp>
#include "Core/App/Event.hpp"
#include "EditorEvent.hpp"

#include "Core/App/Application.hpp"
#include "Core/App/EventBus.hpp"

#include "FrameCounter.hpp"
#include "SelectionBox.hpp"
#include "Core/App/Clipboard.hpp"

#include <set>

namespace mm
{
	class Animation;
	class Model;
	class EditorLayer;

	class Sequencer
	{
		static constexpr ImVec2 BUTTON_SIZE = ImVec2(10, 10);
		static constexpr uint32_t BUTTON_COLOR = 0x7fc0c0c0;
		static constexpr uint32_t ROW_HEIGHT = 20;
		static constexpr uint32_t BACKGROUND_COLOR = 0xff242424;
		static constexpr uint32_t HEADER_COLOR = 0xff3d3837;
		static constexpr uint32_t LEGEND_LENGTH = 200;
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
		static constexpr uint32_t DOPE_OUTLINE_COLOR_SELECTED = 0xff0080ff;
		static constexpr float DOPE_OUTLINE_SIZE = 1.5f;
		static constexpr float DOPE_OUTLINE_SIZE_SELECTED = 3.0f;

		static constexpr float POINT_RADIUS = 4.5f;
		static constexpr uint32_t POINT_FILL_COLOR = 0xff000000;
		static constexpr uint32_t POINT_OUTLINE_COLOR = 0xff0080ff;
		static constexpr float POINT_OUTLINE_SIZE = 3.f;
		static constexpr float Y_GAIN = 5.f;
		static constexpr uint32_t INDENT_BASE = 15;

		static constexpr uint32_t CURVE_EDITOR_ROW_COUNT = 16;
		static constexpr int32_t BEZIER_EDITOR_SIZE = 250;
		static constexpr uint32_t BEZIER_EDITOR_GRID_COUNT = 5;

	public:
		struct Group;
		struct Item {
			uint32_t groupIndex;
			std::string name;
			int32_t type;
			int32_t index;
			bool expanded = false;

			/* For drawing */
			int32_t rowIndex;
		};

		struct Group {
			std::string name;
			bool expanded = false;
			std::vector<Item> items;

			/* For drawing */
			int32_t rowIndex;
		};

	public:
		Sequencer(EditorLayer& editor) : 
			m_editor(editor),
			m_listener(EventBus::Instance()) {
			/* Should use ImGui inputs */
			m_listener.listen<Event::MouseScrolled>(MM_EVENT_FN(Sequencer::OnMouseScrolled));
			m_listener.listen<EditorEvent::ItemSelected>(MM_EVENT_FN(Sequencer::OnItemSelected));
		}

		void OnUIRender();
		void AddGroup(const Group& group) { m_groups.push_back(group); }
		void SetModel(Model* model);

		//void OnModelLoaded(const EditorEvent::ModelLoaded& e);
		//void OnMotionLoaded(const EditorEvent::MotionLoaded& e);
		void OnUpdate(float deltaTime);
		void UpdateAnim();
		Model* GetModel() { return m_model; }
		FrameCounter& GetFrameCounter() { return m_frameCounter; }

	private:
		template <typename T>
		decltype(auto) GetFirstKeyframeToDraw(std::vector<T>& keyframeList);

		void CurveEditor(Item& item);

		/* Events */
		void OnMouseScrolled(const Event::MouseScrolled& e);
		void OnItemSelected(const EditorEvent::ItemSelected& e);
		void ProcessKeys();
		void ProcessMouseScroll();

		/* Drawing */
		void DrawExpandButton(uint32_t rowIndex, float offsetX, bool& expanded);
		void DrawDiamond(const ImVec2& center, float radius, float outlineSize, uint32_t outlineColor, uint32_t fillColor);
		template<typename T>
		void DrawRowHeader(T& row, bool expandable, float textOffset);
		/* Morph and bone keyframes should be treated the same */
		template<typename T>
		void DrawItemDope(const Item& item, std::vector<T>& keyframeList);

		void DrawGroupDope(const Group& group);
		void DrawStrip(uint32_t row);
		void DrawScale();
		void DrawRows();
		void DrawExpandedGroup(Group& group);
		bool IsKeyframeSelected(Animation::Keyframe& keyframe);

		const char* GenButtonId() {
			static char buf[16];
			std::snprintf(buf, sizeof(buf), "##B%u", m_buttonIndex++);
			return buf;
		}

	private:
		enum class State {
			IDLE,
			CHERRY_PICK,
			BOX_SELECT,
			DOPE_DRAG
		};

	private:
		EditorLayer& m_editor;
		Model* m_model = nullptr;

		std::vector<Group> m_groups;

		SelectionBox m_selectionBox;

		/* Editing states */
		std::unordered_set<Animation::Keyframe*> m_selectedKeyframes;

		/* Drawing */
		int32_t m_selectedRow = -1;
		int32_t m_selectedColumn = -1;
		bool m_hovered = false;
		ImDrawList* m_drawList = nullptr;
		int32_t m_rowStart = 1;
		int32_t m_currRowIndex;
		int32_t m_totalRowCount = 0;
		int32_t m_visibleRowCount = 0;
		int32_t m_buttonIndex = 0;
		ImVec2 m_canvasMin;
		ImVec2 m_canvasMax;
		ImVec2 m_canvasOrigin;
		ImVec2 m_canvasSize;
		int32_t m_minFrame = 0;
		int32_t m_maxFrame = 0;

		/* Playback states */
		bool m_playing = false;
		FrameCounter m_frameCounter;

		State m_state = State::IDLE;

		/* Editing */
		std::unordered_map<Animation::Keyframe*, uint32_t> m_keyframeFrameOnStartDragging;
		bool m_lastFrameMouseDragged = false;
		bool m_thisFrameMouseDragged = false;
		bool m_thisFrameAnyDopeClicked = false;
		bool m_thisFrameAnyDopeHovered = false;
		Animation::Keyframe* m_currSelectedKeyframe = nullptr;

		bool m_draggingKeyframes;
		float m_mouseXWhenStartDragging = 0.0f;

		dexode::EventBus::Listener m_listener;
	};

	struct SequencerClipboardContent : public ClipboardContent {
		struct Item {
		//	Sequencer::Dope dope;
			//Animation::Keyframe* keyframe;
		};

		std::vector<Item> items;
	};
}

