#pragma once

#include <ImGuizmo.h>
#include "Core/App/Event.hpp"

#include <dexode/EventBus.hpp>

#include "EditorEvent.hpp"
#include "SelectionBox.hpp"

#include "Core/App/Clipboard.hpp"

namespace mm
{
	class Model;
	class EditorLayer;

	struct PoseEditorClipboardContent : public ClipboardContent {
		struct Item {
			int32_t boneIndex;
			Transform transform;
		};

		std::vector<Item> items;
	};
	
	class PoseEditor
	{
		static constexpr uint32_t UNSELECTED_OUTLINE_COLOR = 0x7fc0c0c0;
		static constexpr uint32_t UNSELECTED_FILL_COLOR = 0x3fc0c0c0;
		static constexpr float UNSELECTED_OUTLINE_SIZE = 1.5f;

		static constexpr uint32_t SELECTED_FILL_COLOR = 0x7f0080ff;
		static constexpr uint32_t SELECTED_OUTLINE_COLOR = 0xff0080ff;
		static constexpr float SELECTED_OUTLINE_SIZE = 3.0f;

		static constexpr uint32_t DIRTY_OUTLINE_COLOR = IM_COL32(0, 255, 0, 255);
		static constexpr uint32_t DIRTY_FILL_COLOR = IM_COL32(0, 255, 0, 127);

		static constexpr float CIRCLE_RADIUS = 7.5f;

	public:
		PoseEditor(EditorLayer& editor);
		void SetModel(Model* model);
		Model* GetModel() { return m_model; }
		void OnUpdate(float deltaTime);
		void OnUIRender();

	private:
		void CommitEdited();
		glm::vec3 WorldToScreen(const glm::vec3& world);
		void EditTransform();
		void DrawBones();

		void CalcStartTransform();

		/* Inputs */
		void ProcessKeys();
		void ProcessMouseButton();

		/* Events */
		void OnItemSelected(const EditorEvent::ItemSelected& e);
		void OnFrameSet(const EditorEvent::FrameSet& e);

	private:
		struct Context {
			enum class State {
				PICKING = 0,
				EDITING
			};

			/* Gizmo */
			State state = State::PICKING;
			uint8_t operation = 0;
			uint8_t mode = 0;

			int32_t currSelectedBone = -1;

			bool thisFrameClickedOnAnyBone = false;
			std::unordered_set<int32_t> selectedBones;
			std::unordered_set<int32_t> editedBones;

			/* Context for currSelectedBone */
			glm::mat4 world = glm::mat4(1);
			glm::mat4 worldToLocal = glm::mat4(1);


			bool useLocalFrame = false;
			bool fixedAxis = false;
			glm::mat4 localFrame; 
			glm::mat4 localFrameInv;

			std::vector<glm::vec3> screenPos;
		};

	private:
		EditorLayer& m_editor;
		Model* m_model = nullptr;

		Context m_context;

		dexode::EventBus::Listener m_listener;

		bool m_enabled = false;
	};
}

