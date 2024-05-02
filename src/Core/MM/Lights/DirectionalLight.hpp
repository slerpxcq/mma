#pragma once

#include "Light.hpp"

namespace mm
{
	class DirectionalLight : public Light
	{
	public:
		DirectionalLight() { m_name = "DirectionalLight0"; }

		void SetColor(const glm::vec3& color) { m_color = color; }
		void SetDirection(const glm::vec3& direction) { m_direction = direction; }
	private:
		glm::vec3 m_color;
		glm::vec3 m_direction;
	};
}
