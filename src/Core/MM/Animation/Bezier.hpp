#pragma once

namespace mm
{
	class Bezier
	{
	public:
		Bezier();
		Bezier(const uint8_t* interp);
		glm::vec4 Eval(float target) const;

	private:
		// x, y, z, R
		// p1, p2
		glm::ivec2 m_handles[4][2];
	};

}
