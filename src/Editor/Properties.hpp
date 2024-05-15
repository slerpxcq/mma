#pragma once

#include "Commands.hpp"
#include "EditorEvent.hpp"

#include "Core/App/EventBus.hpp"

namespace mm
{
	class EditorLayer;

	class Entity;
	class Model;
	class World;

	class Properties
	{
	public:
		Properties(EditorLayer& editor);
		void OnUIRender();

	private:
		void OnEntitySelected(const EditorEvent::EntitySelected& e);

		template <typename T>
		void ShowPanel();

		template <typename T>
		void ShowPanelCore(T& e);

	private:
		EditorLayer& m_editor;

		Entity* m_entity = nullptr;

		dexode::EventBus::Listener m_listener;
	};
}

