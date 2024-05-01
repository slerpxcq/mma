#pragma once

#include "Core/MM/Math/Transform.hpp" 

#include "Core/MM/Animation/Animation.hpp"

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
			enum {
				TYPE_BONE,
				TYPE_MORPH
			};

		public:
			KeyframeInserted(Animation& anim, uint8_t type, uint32_t index, uint32_t frame) :
				m_animation(anim),
				m_type(type),
				m_index(index),
				m_frame(frame) {}

			virtual void Undo() override { 
				MM_INFO("{0}", __FUNCTION__);
				switch (m_type) {
				case TYPE_BONE:
					m_animation.RemoveBoneKeyframe(m_index, m_frame);
					break;
				case TYPE_MORPH:
					m_animation.RemoveMorphKeyframe(m_index, m_frame);
					break;
				}
			}

			virtual void Redo() override {}

		private:
			Animation& m_animation;
			uint8_t m_type;
			uint32_t m_index;
			uint32_t m_frame;
		};

		class KeyframeMoved : public ICommand {

		};

		class KeyframeDeleted : public ICommand {

		};
	}
}
