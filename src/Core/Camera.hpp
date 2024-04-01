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
		static constexpr float FOV = glm::radians(45.f);
		static constexpr float Z_NEAR = 0.1f;
		static constexpr float Z_FAR = 100.f;
	public:
		Camera();
		~Camera();

		const glm::mat4& GetView() const {
			static glm::mat4 view;
			view = glm::lookAt(m_eye, m_center, m_up);
			return view;
		}

		const glm::mat4& GetProj() const {
			static glm::mat4 proj;
			proj = glm::perspective(FOV, m_aspect, Z_NEAR, Z_FAR);
			return proj;
		}

		void Update();
		void OnMouseButtonPressed(const Event::MouseButtonPressed& e);
		void OnMouseScrolled(const Event::MouseScrolled& e);
		void OnWindowResized(const Event::WindowSized& e);

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

		float m_aspect;
	};

}
