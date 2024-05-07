#pragma once

#include <dexode/EventBus.hpp>
#include "Core/App/Event.hpp"
#include "EditorEvent.hpp"

#include "Core/App/Application.hpp"
#include "Core/App/EventBus.hpp"

#include "FrameCounter.hpp"
#include "SelectionBox.hpp"
#include "Clipboard.hpp"

#include <set>

namespace mm
{
	class Animation;
	class Model;
	class EditorLayer;

	template <typename T>
	void DuplicateKeyframe(Animation::KeyframeContainer<T>& container, const T& keyframe, uint32_t frame)
	{
		auto it = LowerBound(container, frame);
		T newKeyframe = keyframe;
		newKeyframe.frame = frame;
		container.insert(it, newKeyframe);

		/* Discard existing keyframe */
		if (it != container.end()) {
			container.erase(it);
		}
	}

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

		//static constexpr float POINT_RADIUS = 4.5f;
		//static constexpr uint32_t POINT_FILL_COLOR = 0xff000000;
		//static constexpr uint32_t POINT_OUTLINE_COLOR = 0xff0080ff;
		//static constexpr float POINT_OUTLINE_SIZE = 3.f;
		//static constexpr float Y_GAIN = 5.f;
		static constexpr uint32_t INDENT_BASE = 15;

		static constexpr uint32_t AUTO_SCROLL_THRESHOLD = 5;
		static constexpr uint32_t AUTO_SCROLL_STEP = 15;

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

		struct DopeBase {
		public:
			DopeBase(Animation::Keyframe* keyframe) :
				keyframe(keyframe) {}

			virtual void Duplicate(uint32_t frame) = 0;
			virtual ~DopeBase() {}

		public:
			Animation::Keyframe* keyframe;
		};

		template <typename T>
		struct Dope : public DopeBase {
			Dope(Animation::KeyframeContainer<T>& container, T* keyframe) :
				DopeBase(keyframe),
				container(container) {}

			virtual void Duplicate(uint32_t frame) override {
				DuplicateKeyframe(container, *dynamic_cast<T*>(keyframe), frame);
			}

			Animation::KeyframeContainer<T>& container;
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
		typename Animation::KeyframeContainer<T>::iterator FirstKeyframeOnCanvas(Animation::KeyframeContainer<T>& keyframeList);

		/* Events */
		void OnMouseScrolled(const Event::MouseScrolled& e);
		void OnItemSelected(const EditorEvent::ItemSelected& e);
		void ProcessKeys();

		void CheckAutoScroll(uint32_t frame);

		void SetFrame(uint32_t frame);

		/* Drawing */
		void DrawExpandButton(uint32_t rowIndex, float offsetX, bool& expanded);
		void DrawDiamond(const ImVec2& center, float radius, float outlineSize, uint32_t outlineColor, uint32_t fillColor);
		template<typename T>
		void DrawRowHeader(T& row, bool expandable, float textOffset);
		/* Morph and bone keyframes should be treated the same */

		template<typename T>
		void DrawItemDope(const Item& item, Animation::KeyframeContainer<T>& keyframeList);

		void DrawGroupDope(const Group& group);
		void DrawStrip(uint32_t row);
		void DrawScale();
		void DrawRows();
		void DrawExpandedGroup(Group& group);

		//template <typename T>
		bool IsKeyframeSelected(Animation::Keyframe*);

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

		struct DopeHash {
			size_t operator()(const std::shared_ptr<Sequencer::DopeBase>& dope) const {
				return std::hash<size_t>()((size_t)dope->keyframe);
			}
		};

	private:
		EditorLayer& m_editor;
		Model* m_model = nullptr;

		std::vector<Group> m_groups;

		SelectionBox m_selectionBox;

		/* Editing states */
		/* Need custom hash function that only hash the underlying keyframe pointer */
		std::unordered_set<std::shared_ptr<DopeBase>, DopeHash> m_selectedDopes;

		/* Drawing */
		int32_t m_selectedRow = -1;
		int32_t m_selectedFrame = -1;
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

		/* Editing */
		State m_state = State::IDLE;
		std::unordered_map<DopeBase*, uint32_t> m_keyframeFrameOnStartDragging;
		bool m_lastFrameMouseDragged = false;
		bool m_thisFrameMouseDragged = false;
		bool m_thisFrameAnyDopeClicked = false;
		bool m_thisFrameAnyDopeHovered = false;

		dexode::EventBus::Listener m_listener;
	};

	struct SequencerClipboardContent : public ClipboardContent {
		std::vector<std::shared_ptr<Sequencer::DopeBase>> dopes;
	};

	/* Hash for selected dopes */
	//static bool operator==(const std::shared_ptr<Sequencer::DopeBase>& lhs, const std::shared_ptr<Sequencer::DopeBase>& rhs) {
	//	return lhs->keyframe == rhs->keyframe;
	//}
}

