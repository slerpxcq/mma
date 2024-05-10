#pragma once

namespace mm
{
	class Bezier
	{
	public:
		Bezier();
		Bezier(const uint8_t* interp);
		glm::vec4 Eval(float target) const;
		const auto& GetHandles() const { return m_handles; }
		auto& GetHandles() { return m_handles; }

	private:
		// x, y, z, R
		// p1, p2
		glm::ivec2 m_handles[4][2];
	};

}
