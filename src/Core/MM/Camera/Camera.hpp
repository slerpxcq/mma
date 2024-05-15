#pragma once

#include "../Entity.hpp"

#include <glm/gtc/matrix_transform.hpp>

namespace mm
{
	class Camera : public Entity {
		friend class CameraController;
		friend class Properties;

		static constexpr float FOV = glm::radians(45.f);
		static constexpr float Z_NEAR = 0.1f;
		static constexpr float Z_FAR = 100.f;

	public:
		Camera() {}

		void SetAspect(float aspect) { m_aspect = aspect; }
		void SetFov(float fov) { m_fov = fov; }

		const glm::mat4& GetView() const {
			static glm::mat4 view;
			view = glm::lookAt(m_eye, m_center, m_up);
			return view;
		}

		const glm::mat4& GetProj() const {
			static glm::mat4 proj;
			proj = glm::perspective(m_fov, m_aspect, Z_NEAR, Z_FAR);
			return proj;
		}

	private:
		glm::vec3 m_eye = glm::vec3(0, 10, -40);
		glm::vec3 m_center = glm::vec3(0, 10, 0);
		glm::vec3 m_up = glm::vec3(0, 1, 0);

		float m_aspect = 1.778f;
		float m_fov = FOV;
	};
}
