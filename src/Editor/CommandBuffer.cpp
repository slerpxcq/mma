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
		/* Discard tail */
		auto newTail = m_container.insert(m_iterator, std::unique_ptr<Command>(e.command));
		m_container.erase(m_iterator, m_container.end());
		m_iterator = std::next(newTail);
		//MM_INFO("Stored command; size={0}", m_container.size());
	}

	void CommandBuffer::OnUndo(const Event::Undo& e)
	{
		if (m_iterator != m_container.begin()) {
			std::advance(m_iterator, -1);
			(*m_iterator)->Undo();
			//MM_INFO("Undo; size={0}", m_container.size());
		}
	}

	void CommandBuffer::OnRedo(const Event::Redo& e)
	{
		if (m_iterator != m_container.end()) {
			(*m_iterator)->Redo();
			std::advance(m_iterator, 1);
			//MM_INFO("Redo; size={0}", m_container.size());
		}
	}
}
