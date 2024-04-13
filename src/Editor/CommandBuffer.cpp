#include "mmpch.hpp"
#include "CommandBuffer.hpp"

#include "Core/App/Application.hpp"

namespace mm
{
	CommandBuffer::CommandBuffer() :
		m_listener(Application::Instance().GetEventBus())
	{
		//m_listener.listen<EditorEvent::CommandIssued>(MM_EVENT_FN(CommandBuffer::OnCommandIssued));
	}

	//void CommandBuffer::OnCommandIssued(const EditorEvent::CommandIssued& e)
	//{
	//	//m_commandQueue.push_back(std::move(e.command));
	//	++m_commandPointer;
	//}

	void CommandBuffer::OnUndo()
	{
		if (m_commandPointer-- >= 0) {
			m_commandQueue[m_commandPointer]->Undo();
		}
	}

	void CommandBuffer::OnRedo()
	{
		if (m_commandPointer++ < m_commandQueue.size()) {
			m_commandQueue[m_commandPointer]->Redo();
		}
	}
}
