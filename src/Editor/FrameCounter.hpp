#pragma once

namespace mm
{
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

}
