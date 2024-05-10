#include "mmpch.hpp"
#include "Bezier.hpp"

namespace mm
{
	glm::vec4 Bezier::Eval(float target) const
	{
		glm::vec4 result;
		for (uint32_t i = 0; i < 4; ++i) 
			result[i] = Eval(i, target);
		return result;
	}

	float Bezier::Eval(uint32_t axis, float target) const
	{
		constexpr float MAX_ERROR = 0.001f;

		float x1 = m_handles[axis][0].x / 127.f;
		float y1 = m_handles[axis][0].y / 127.f;
		float x2 = m_handles[axis][1].x / 127.f;
		float y2 = m_handles[axis][1].y / 127.f;

		float u = 0.5f;
		float low = 0.0f;
		float high = 1.0f;
		float u2, u3, b1, b2, b3, fX, fY;
		float mid;

		do {
			u2 = u * u;
			u3 = u2 * u;
			b1 = 3 * u - 6 * u2 + 3 * u3;
			b2 = 3 * u2 - 3 * u3;
			b3 = u3;
			fX = b1 * x1 + b2 * x2 + b3;

			mid = 0.5f * (low + high);

			if (fX < target)
				low = mid;
			else
				high = mid;

			u = 0.5f * (low + high);

		} while (std::fabs(target - fX) > MAX_ERROR);

		fY = b1 * y1 + b2 * y2 + b3;

		return fY;
	}

	Bezier::Bezier()
	{
		for (uint32_t i = 0; i < 4; ++i) {
			m_handles[i][0] = glm::ivec2(20, 20);
			m_handles[i][1] = glm::ivec2(107, 107);
		}
	}

	Bezier::Bezier(const uint8_t* interp)
	{
		uint8_t (*mem)[16] = (decltype(mem))interp;
		for (uint32_t i = 0; i < 4; ++i) {
			m_handles[i][0].x = mem[i][0];
			m_handles[i][0].y = mem[i][4];
			m_handles[i][1].x = mem[i][8];
			m_handles[i][1].y = mem[i][12];
		}
	}
}

