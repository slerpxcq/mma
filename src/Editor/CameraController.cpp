#include "mmpch.hpp"
#include "CameraController.hpp"

#include "Core/App/Input.hpp"

#include <glm/gtc/quaternion.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

#include "Core/App/Application.hpp"
#include "Core/App/EventBus.hpp"
#include "Viewport.hpp"

namespace mm
{
	CameraController::CameraController(Viewport& viewport, Camera& camera) :
		m_viewport(viewport),
		m_camera(camera),
		m_listener(EventBus::Instance())
	{
		m_listener.listen<Event::MouseButtonPressed>(MM_EVENT_FN(CameraController::OnMouseButtonPressed));
		m_listener.listen<Event::MouseScrolled>(MM_EVENT_FN(CameraController::OnMouseScrolled));
	}

	void CameraController::Update()
	{
		if (!m_viewport.IsHovered())
			return;

		m_view = glm::normalize(m_camera.m_center - m_camera.m_eye);
		m_right = glm::cross(m_camera.m_up, m_view);

		if (Input::MouseButtonPressed(GLFW_MOUSE_BUTTON_MIDDLE))
		{
			m_mousePosWhenMouseReleased = Input::MousePos();
			glm::vec2 delta = m_mousePosWhenMouseReleased - m_mousePosWhenMouseClicked;
			std::swap(delta.x, delta.y);
			glm::vec3 translation = PAN_SPEED * (delta.x * m_camera.m_up + delta.y * m_right);
			m_camera.m_eye = m_eyeOnWhenMouseClicked + translation;
			m_camera.m_center = m_centerWhenMouseClicked + translation;
		}

		if (Input::MouseButtonPressed(GLFW_MOUSE_BUTTON_RIGHT)) {
			m_mousePosWhenMouseReleased = Input::MousePos();
			glm::vec2 delta = (m_mousePosWhenMouseReleased - m_mousePosWhenMouseClicked);
			std::swap(delta.x, delta.y);
			delta *= ORBIT_SPEED;
			glm::quat qy = glm::angleAxis(-delta.y, glm::vec3(0, 1, 0));
			glm::quat qx = glm::angleAxis(delta.x, m_rightWhenMouseClicked);
			glm::quat q = qy * qx;
			m_camera.m_eye = glm::rotate(q, m_eyeOnWhenMouseClicked);
			m_camera.m_center = glm::rotate(q, m_centerWhenMouseClicked);
			m_camera.m_up = glm::rotate(q, m_upWhenMouseClicked);
		}
	}

	void CameraController::OnMouseButtonPressed(const Event::MouseButtonPressed& e)
	{
		if (!m_viewport.IsHovered())
			return;

		switch (e.button) {
		case GLFW_MOUSE_BUTTON_RIGHT:
		case GLFW_MOUSE_BUTTON_MIDDLE:
			{
				m_mousePosWhenMouseClicked = m_mousePosWhenMouseReleased = Input::MousePos();
				m_eyeOnWhenMouseClicked = m_camera.m_eye;
				m_centerWhenMouseClicked = m_camera.m_center;
				m_upWhenMouseClicked = m_camera.m_up;
				m_rightWhenMouseClicked = m_right;
				break;
			}
		}
	}

	void CameraController::OnMouseScrolled(const Event::MouseScrolled& e)
	{
		if (!m_viewport.IsHovered())
			return;

		glm::vec3 translation = ZOOM_SPEED * e.delta * m_view;
		m_camera.m_eye += translation;
		m_camera.m_center += translation;
	}
}

