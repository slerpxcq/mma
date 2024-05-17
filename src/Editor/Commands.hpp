#pragma once

namespace mm
{
	class Command {
	public:
		virtual void Undo() = 0;
		virtual void Redo() = 0;
		virtual const char* What() { return ""; }
		virtual ~Command() {}
	};

	template <typename T>
	class ValueEditedCommand : public Command {
	public:
		ValueEditedCommand(T& ref, T undoValue, T redoValue) : 
			m_ref(ref),
			m_undoValue(undoValue),
			m_redoValue(redoValue) {}

		virtual void Undo() override { m_ref = m_undoValue; }
		virtual void Redo() override { m_ref = m_redoValue; }

	private:
		T& m_ref;
		T m_undoValue;
		T m_redoValue;
	};
}
