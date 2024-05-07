#pragma once

#include <dexode/EventBus.hpp>

#include "Core/App/Event.hpp"
#include "EditorEvent.hpp"
#include "Commands.hpp"

namespace mm
{
	class CommandBuffer
	{
	public:
		CommandBuffer();

	private:
		void OnCommandIssued(const EditorEvent::CommandIssued& e);
		void OnUndo(const Event::Undo& e);
		void OnRedo(const Event::Redo& e);

	private:
		dexode::EventBus::Listener m_listener;

		//int32_t m_nextInsert = 0;
		std::stack<std::unique_ptr<Command>> m_commandQueue;
	};
}

