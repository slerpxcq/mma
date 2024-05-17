#pragma once

#include <dexode/EventBus.hpp>

#include "Core/App/Event.hpp"
#include "EditorEvent.hpp"
#include "Commands.hpp"

namespace mm
{
	class CommandBuffer
	{
		using CommandContainer = std::list<std::unique_ptr<Command>>;
		using CommandIterator = CommandContainer::iterator;
	public:
		CommandBuffer();

	private:
		void OnCommandIssued(const EditorEvent::CommandIssued& e);
		void OnUndo(const Event::Undo& e);
		void OnRedo(const Event::Redo& e);

	private:
		CommandContainer m_container;
		CommandIterator m_iterator;

		dexode::EventBus::Listener m_listener;
	};
}

