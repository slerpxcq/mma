#pragma once

#include "Core/MM/Transform.hpp"

namespace mm
{
	class ICommand {
	public:
		virtual void Undo() {};
		virtual void Redo() {};
		virtual ~ICommand() {}
	};

	namespace Command 
	{
		class MorphEdited : public ICommand {
		public:
			MorphEdited(float* valuePtr, float redoValue, float undoValue) :
				m_valuePtr(valuePtr), m_redoValue(redoValue), m_undoValue(undoValue) {}

			virtual void Undo() override { *m_valuePtr = m_undoValue; }
			virtual void Redo() override { *m_valuePtr = m_redoValue; }

		private:
			float* m_valuePtr;
			float m_redoValue;
			float m_undoValue;
		};

		class BoneEdited : public ICommand {
		public:
			BoneEdited(Transform* valuePtr, Transform redoValue, Transform undoValue) :
				m_valuePtr(valuePtr), m_redoValue(redoValue), m_undoValue(undoValue) {}

			virtual void Undo() override { *m_valuePtr = m_undoValue; }
			virtual void Redo() override { *m_valuePtr = m_redoValue; }

		private:
			Transform* m_valuePtr;
			Transform m_redoValue;
			Transform m_undoValue;
		};

		class KeyframeInserted : public ICommand {
		public:
		private:
		};
	}
}
