#pragma once

#include "EditorEvent.hpp"
#include "Core/App/EventBus.hpp"

namespace mm
{
	class EditorLayer;
	class Model;
	class World;

	class MorphEditedCommand : public Command {
	public:
		MorphEditedCommand(float* valuePtr, float redoValue, float undoValue) :
			m_valuePtr(valuePtr), m_redoValue(redoValue), m_undoValue(undoValue) {}

		virtual void Undo() override { *m_valuePtr = m_undoValue; }
		virtual void Redo() override { *m_valuePtr = m_redoValue; }

	private:
		float* m_valuePtr;
		float m_redoValue;
		float m_undoValue;
	};

	class Properties
	{
	public:
		enum {
			TYPE_BONE,
			TYPE_MESH,
			TYPE_CAMERA,
			TYPE_LIGHT,
			TYPE_MORPH,
			TYPE_ANIMATION,
			TYPE_ARMATURE,
			TYPE_WORLD,
			TYPE_PHYSICS_WORLD,
			TYPE_MODEL
		} ItemType;

		struct Item {
			std::string name;
			uint32_t index;
			uint8_t type;
			std::any data;
		};

	public:
		Properties(EditorLayer& editor);
		void OnUIRender();

	private:
		void LoadModel(World& world);
		void LoadAnimation(Model&);
		void ExportAnimation(Model&);
		void MorphSliders(Model& model, uint32_t panel);
		void OnItemSelected(const EditorEvent::ItemSelected& e);
		void MeshPanel();
		void BonePanel();
		void MorphPanel();
		void AnimationPanel();
		void PhysicsWorldPanel();
		void WorldPanel();
		void LightPanel();

	private:
		EditorLayer& m_editor;

		dexode::EventBus::Listener m_listener;

		/* Context */
		std::any m_item;
		uint8_t m_type;
	};
}

