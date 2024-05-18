#pragma once

#include "Light.hpp"

namespace mm
{
	class DirectionalLight : public Light
	{
		friend class Properties;
	public:
		DirectionalLight();

		void SetColor(const glm::vec3& color) { m_color = color; }
		void SetDirection(const glm::vec3& direction) { m_direction = direction; }

		const glm::vec3& GetColor() const { return m_color; }
		const glm::vec3& GetDirection() const { return m_direction; }

	private:
		glm::vec3 m_color;
		glm::vec3 m_direction;
	};
}

