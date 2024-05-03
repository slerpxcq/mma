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
	}

	void CommandBuffer::OnCommandIssued(const EditorEvent::CommandIssued& e)
	{
		m_commandQueue.push(std::unique_ptr<ICommand>(e.command));
		MM_INFO("Stored command; size={0}", m_commandQueue.size());
	}

	void CommandBuffer::OnUndo(const Event::Undo& e)
	{
		if (!m_commandQueue.empty()) {
			MM_INFO("Undo; index={0}", m_commandQueue.size()-1);
			m_commandQueue.top()->Undo();
			m_commandQueue.pop();
		}
	}

	void CommandBuffer::OnRedo(const Event::Redo& e)
	{
		MM_INFO("TO BE IMPLEMENTED");
	}
}
