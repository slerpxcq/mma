#pragma once

#include "EditorEvent.hpp"
#include "Core/App/EventBus.hpp"

namespace mm
{
	class EditorLayer;

	class Properties
	{
	public:
		enum {
			TYPE_BONE,
			TYPE_MESH,
			TYPE_CAMERA,
			TYPE_LIGHT,
			TYPE_MORPH,
			TYPE_ANIMATION
		} ItemType;

	public:
		Properties(EditorLayer& editor);
		void OnUIRender();

	private:
		void MorphSliders(Model& model, uint32_t panel);
		void OnItemSelected(const EditorEvent::ItemSelected& e);

	private:
		EditorLayer& m_editor;

		dexode::EventBus::Listener m_listener;

		/* Context */
		void* m_item = nullptr;
		uint8_t m_type;
	};
}

