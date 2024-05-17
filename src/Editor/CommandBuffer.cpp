#include "mmpch.hpp"
#include "CommandBuffer.hpp"

#include "Core/App/Application.hpp"
#include "Core/App/EventBus.hpp"
#include "Core/App/Event.hpp"

namespace mm
{
	CommandBuffer::CommandBuffer() :
		m_listener(EventBus::Instance())
	{
		m_listener.listen<EditorEvent::CommandIssued>(MM_EVENT_FN(CommandBuffer::OnCommandIssued));
		m_listener.listen<Event::Undo>(MM_EVENT_FN(CommandBuffer::OnUndo));
		m_listener.listen<Event::Redo>(MM_EVENT_FN(CommandBuffer::OnRedo));
		m_iterator = m_container.begin();
	}

	void CommandBuffer::OnCommandIssued(const EditorEvent::CommandIssued& e)
	{
		m_container.push_back(std::unique_ptr<Command>(e.command));
		m_iterator = std::prev(m_container.end());
		MM_INFO("Stored command; size={0}", m_container.size());
	}

	void CommandBuffer::OnUndo(const Event::Undo& e)
	{
		(*m_iterator)->Undo();
		if (m_iterator != m_container.begin())
			std::advance(m_iterator, -1);
	}

	void CommandBuffer::OnRedo(const Event::Redo& e)
	{
		(*m_iterator)->Redo();
		if (m_iterator != std::prev(m_container.end()))
			std::advance(m_iterator, 1);
	}
}
