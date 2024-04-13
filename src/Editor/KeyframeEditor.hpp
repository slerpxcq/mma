#pragma once

#include "EditorEvent.hpp"
#include "Sequencer.hpp"

#include <dexode/EventBus.hpp>

namespace mm
{
	class EditorLayer;
	class Model;
	class MySequencer;

	class FrameCounter
	{
	public:
		void Set(uint32_t frame) {
			m_subframe = 0;
			m_frame = frame;
		}

		void Step(float deltaTime) {
			static float accum;
			accum += deltaTime;
			while (accum >= SUBFRAME_STEP) {
				if (++m_subframe == SUBFRAME_COUNT) {
					m_subframe = 0;
					++m_frame;
				}
				accum -= SUBFRAME_STEP;
			}
		}

		uint32_t GetFrame() { return m_frame; }
		uint32_t GetSubFrame() { return m_subframe; }

	private:
		static constexpr float SUBFRAME_STEP = 1.f / 240;
		static constexpr uint32_t SUBFRAME_COUNT = 8;
	
	private:
		uint32_t m_frame;
		uint32_t m_subframe;
	};

	class KeyframeEditor
	{
		friend class Sequencer;
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
		Sequencer m_sequencer;

		bool m_playing = false;
		//int32_t m_frame = 0;
		//int32_t m_subframe = 0;
		FrameCounter m_frameCounter;

		dexode::EventBus::Listener m_listener;
	};
}

