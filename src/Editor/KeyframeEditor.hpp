#pragma once

#include "EditorEvent.hpp"
#include "Sequencer.hpp"

#include <dexode/EventBus.hpp>

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
		void OnModelLoaded(const EditorEvent::ModelLoaded& e);
		void OnMotionLoaded(const EditorEvent::MotionLoaded& e);
		void OnUpdate(float deltaTime);
		void UpdateAnim();
		void OnUIRender();
		Model* GetModel() { return m_model; }

	private:
		EditorLayer& m_editor;
		Model* m_model = nullptr;

		bool m_playing = false;
		int32_t m_frame = 0;
		int32_t m_subframe = 0;

		dexode::EventBus::Listener m_listener;

		std::vector<MySequence> m_clusters;
	};

}

