#pragma once

#include <glm/gtc/matrix_transform.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

#include "Core/Event.hpp"
#include "Core/EventUser.hpp"

namespace mm
{
	class Camera : public EventUser
	{
		static constexpr float PAN_SPEED = 0.01f;
		static constexpr float ORBIT_SPEED = 0.005f;
		static constexpr float ZOOM_SPEED = 1.f;
	public:
		Camera();
		~Camera();


		glm::mat4 GetMatrix() const {
			glm::mat4 matrix = glm::lookAt(m_eye, m_center, m_up);
			return matrix;
		}

		void Update();
		void OnMouseButtonPressed(const Event::MouseButtonPressed& e);
		void OnMouseScrolled(const Event::MouseScrolled& e);

	private:
		glm::vec2 GetMousePos();

	private:
		glm::vec3 m_view;
		glm::vec3 m_eye;
		glm::vec3 m_center;
		glm::vec3 m_up;
		glm::vec3 m_right;

		// states
		glm::vec2 m_mousePos0;
		glm::vec2 m_mousePos1;
		glm::vec3 m_eye0;
		glm::vec3 m_center0;
		glm::vec3 m_up0;
		glm::vec3 m_right0;
	};

}
