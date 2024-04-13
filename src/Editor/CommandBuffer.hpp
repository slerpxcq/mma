#pragma once

#include <dexode/EventBus.hpp>

#include "EditorEvent.hpp"

namespace mm
{
	class Command 
	{
	public:
		virtual void Undo() = 0;
		virtual void Redo() = 0;
		virtual ~Command() {}
	};

	class BoneTransformedCommand : public Command
	{
	public:
		BoneTransformedCommand(int32_t index, const Transform& transform) : 
			m_index(index), 
			m_transform(transform) {}

		virtual void Undo() override {
			Transform inverse = m_transform.inverse();
		}

		virtual void Redo() override {
		}

	private:
		int32_t m_index;
		Transform m_transform;
	};

	class CommandBuffer
	{
	public:
		CommandBuffer();

		//void OnCommandIssued(const EditorEvent::CommandIssued& e);

	private:
		void OnUndo();
		void OnRedo();

	private:
		dexode::EventBus::Listener m_listener;

		int32_t m_commandPointer = -1;
		std::vector<std::unique_ptr<Command>> m_commandQueue;
	};
}

