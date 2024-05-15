#pragma once

#include "Core/App/Event.hpp"
#include "Core/MM/Animation/Animation.hpp"	

#include "Commands.hpp"
#include "EditorEvent.hpp"
#include "SelectionBox.hpp"
#include "Clipboard.hpp"

#include <ImGuizmo.h>

namespace mm
{
	class Model;
	class EditorLayer;

	//class PoseEntity : public Entity {
	//public:
	//	PoseEntity(const std::string& name, Transform& pose) :
	//		Entity(name),
	//		m_pose(pose) {}

	//	virtual void PropertiesPanel() override {
	//		ImGui::Text(m_name.c_str());
	//	}

	//private:
	//	Transform& m_pose;
	//};

	struct PoseEditorClipboardContent : public ClipboardContent {
	public:
		// Better use bone name for inter-model paste?
		struct Item {
			//int32_t boneIndex;
			std::string boneName;
			Transform transform;
		};
	public:
		std::vector<Item> items;
	};

	class PoseEditorPasteCommand : public Command {
	};

	class PoseEditorCommitCommand : public Command {
	public:
		enum {
			TYPE_BONE,
			TYPE_MORPH
		};

	public:
		PoseEditorCommitCommand(Animation& anim, uint8_t type, uint32_t index, uint32_t frame) :
			m_animation(anim),
			m_type(type),
			m_index(index),
			m_frame(frame) {}

		virtual void Undo() override { 
			MM_INFO("{0}", __FUNCTION__);
			switch (m_type) {
			case TYPE_BONE:
				m_animation.RemoveBoneKeyframe(m_index, m_frame);
				break;
			case TYPE_MORPH:
				m_animation.RemoveMorphKeyframe(m_index, m_frame);
				break;
			}
		}

		virtual void Redo() override {}

	private:
		Animation& m_animation;
		uint8_t m_type;
		uint32_t m_index;
		uint32_t m_frame;
	};

	class PoseEditorEditedCommand : public Command {
	public:
		PoseEditorEditedCommand(Transform* valuePtr, Transform redoValue, Transform undoValue) :
			m_valuePtr(valuePtr), m_redoValue(redoValue), m_undoValue(undoValue) {}

		virtual void Undo() override { *m_valuePtr = m_undoValue; }
		virtual void Redo() override { *m_valuePtr = m_redoValue; }

	private:
		Transform* m_valuePtr;
		Transform m_redoValue;
		Transform m_undoValue;
	};
	
	class PoseEditor
	{
		static constexpr uint32_t UNSELECTED_OUTLINE_COLOR = 0x7fc0c0c0;
		static constexpr uint32_t UNSELECTED_FILL_COLOR = 0x3fc0c0c0;
		static constexpr float UNSELECTED_OUTLINE_SIZE = 1.5f;

		static constexpr uint32_t SELECTED_FILL_COLOR = 0x7f0080ff;
		static constexpr uint32_t SELECTED_OUTLINE_COLOR = 0xff0080ff;
		static constexpr float SELECTED_OUTLINE_SIZE = 3.0f;

		static constexpr uint32_t DIRTY_OUTLINE_COLOR = 0xff00407f;
		static constexpr uint32_t DIRTY_FILL_COLOR = 0x7f00407f;

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
		void OnItemSelected(const EditorEvent::EntitySelected& e);
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

