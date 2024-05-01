#pragma once

#include "Light.hpp"

namespace mm
{
	class PointLight : public Light
	{
	public:
	private:
		glm::vec3 m_color;
		glm::vec3 m_intensity;
		glm::vec3 m_attenuation;
	};
}

