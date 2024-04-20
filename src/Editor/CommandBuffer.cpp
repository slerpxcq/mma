#include "mmpch.hpp"
#include "CommandBuffer.hpp"

#include "Core/App/Application.hpp"
#include "Core/App/Event.hpp"

namespace mm
{
	CommandBuffer::CommandBuffer() :
		m_listener(Application::Instance().GetEventBus())
	{
		m_listener.listen<EditorEvent::CommandIssued>(MM_EVENT_FN(CommandBuffer::OnCommandIssued));
		m_listener.listen<Event::KeyPressed>(MM_EVENT_FN(CommandBuffer::OnKeyPressed));
	}

	void CommandBuffer::OnCommandIssued(const EditorEvent::CommandIssued& e)
	{
		m_commandQueue.push_back(std::unique_ptr<ICommand>(e.command));
		++m_commandPointer;
		MM_INFO("Recieved command; pointer={0}", m_commandPointer);
	}

	void CommandBuffer::OnKeyPressed(const Event::KeyPressed& e)
	{
		// Ctrl + Z
		if (e.code == GLFW_KEY_Z && (e.mods & GLFW_MOD_CONTROL)) {
			OnUndo();
		}

		// Ctrl + Y
		if (e.code == GLFW_KEY_Y && (e.mods & GLFW_MOD_CONTROL)) {
			OnRedo();
		}
	}

	void CommandBuffer::OnUndo()
	{
		if (m_commandPointer >= 0) {
			MM_INFO("Undo; pointer={0}", m_commandPointer);
			m_commandQueue[m_commandPointer]->Undo();
			--m_commandPointer;
		}
	}

	void CommandBuffer::OnRedo()
	{
		if (m_commandPointer < m_commandQueue.size()) {
			MM_INFO("Redo; pointer={0}", m_commandPointer);
			m_commandQueue[m_commandPointer]->Redo();
			++m_commandPointer;
		}
	}
}
