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
		void OnKeyPressed(const Event::KeyPressed& e);
		void OnUndo();
		void OnRedo();

	private:
		dexode::EventBus::Listener m_listener;

		int32_t m_commandPointer = -1;
		std::vector<std::unique_ptr<ICommand>> m_commandQueue;
	};
}

