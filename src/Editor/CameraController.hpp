#pragma once

#include "Core/App/Event.hpp"
#include "Core/App/EventUser.hpp"

#include "Core/MM/Camera/Camera.hpp"

namespace mm
{
	class Camera;

	class CameraController : public EventUser
	{
		static constexpr float PAN_SPEED = 0.01f;
		static constexpr float ORBIT_SPEED = 0.005f;
		static constexpr float ZOOM_SPEED = 1.f;
	public:
		CameraController(Camera&);
		void Update();
		Camera& GetCamera() { return m_camera; }

	private:
		void OnMouseButtonPressed(const Event::MouseButtonPressed& e);
		void OnMouseScrolled(const Event::MouseScrolled& e);

	private:
		Camera& m_camera;

		// helper states
		glm::vec3 m_view;
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

