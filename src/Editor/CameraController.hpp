#pragma once

#include "Core/App/Event.hpp"
#include "Core/MM/Camera/Camera.hpp"

#include <dexode/EventBus.hpp>

namespace mm
{
	class Camera;
	class Viewport;

	class CameraController
	{
		static constexpr float PAN_SPEED = 0.01f;
		static constexpr float ORBIT_SPEED = 0.005f;
		static constexpr float ZOOM_SPEED = 1.f;
	public:
		CameraController(Viewport&, Camera&);
		void Update();
		Camera& GetCamera() { return m_camera; }

	private:
		void OnMouseButtonPressed(const Event::MouseButtonPressed& e);
		void OnMouseScrolled(const Event::MouseScrolled& e);

	private:
		Viewport& m_viewport;

		dexode::EventBus::Listener m_listener;

		Camera& m_camera;

		/* helper states */
		glm::vec3 m_view;
		glm::vec3 m_right;

		glm::vec2 m_mousePosWhenMouseClicked;
		glm::vec2 m_mousePosWhenMouseReleased;
		glm::vec3 m_eyeOnWhenMouseClicked;
		glm::vec3 m_centerWhenMouseClicked;
		glm::vec3 m_upWhenMouseClicked;
		glm::vec3 m_rightWhenMouseClicked;
	};
}

