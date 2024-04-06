#pragma once

#include "Sequencer.hpp"

namespace mm
{
	class EditorLayer;
	class Model;
	class MySequencer;

	class KeyframeEditor
	{
		static constexpr float SUBFRAME_STEP = 1.f / 240;
		static constexpr uint32_t SUBFRAME_COUNT = 8;
	public:
		KeyframeEditor(EditorLayer& editor);
		void OnUpdate(float deltaTime);
		void UpdateAnim();
		void OnUIRender();
		void SetModel(Model* model);
		Model* GetModel() { return m_model; }

	private:
		EditorLayer& m_editor;
		Model* m_model = nullptr;

		bool m_playing = false;
		int32_t m_frame = 0;
		int32_t m_subframe = 0;

		MySequence m_sequence;
	};

}

