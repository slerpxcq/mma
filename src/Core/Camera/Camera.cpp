#include "mmpch.hpp"
#include "Camera.hpp"

#include "Core/App/Application.hpp"
#include "Core/App/Input.hpp"

#include <glm/gtc/quaternion.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

namespace mm
{
	Camera::Camera() :
		m_eye(glm::vec3(0, 10, -40)),
		m_center(glm::vec3(0, 10, 0)),
		m_up(glm::vec3(0, 1, 0)),
		m_view(glm::normalize(m_center - m_eye)),
		m_right(glm::cross(m_up, m_view)),
		m_aspect(1.778f)
	{
		m_listener.listen<Event::MouseScrolled>(MM_EVENT_FN(Camera::OnMouseScrolled));
		m_listener.listen<Event::MouseButtonPressed>(MM_EVENT_FN(Camera::OnMouseButtonPressed));
	}

	Camera::~Camera()
	{
	}

	void Camera::Update()
	{
		m_view = glm::normalize(m_center - m_eye);
		m_right = glm::cross(m_up, m_view);

		if (Input::MouseButtonPressed(GLFW_MOUSE_BUTTON_MIDDLE))
		{
			m_mousePos1 = Input::MousePos();
			glm::vec2 delta = m_mousePos1 - m_mousePos0;
			std::swap(delta.x, delta.y);
			glm::vec3 trans = PAN_SPEED * (delta.x * m_up + delta.y * m_right);
			m_eye = m_eye0 + trans;
			m_center = m_center0 + trans;
		}

		if (Input::MouseButtonPressed(GLFW_MOUSE_BUTTON_RIGHT)) {
			m_mousePos1 = Input::MousePos();
			glm::vec2 delta = (m_mousePos1 - m_mousePos0);
			std::swap(delta.x, delta.y);
			delta *= ORBIT_SPEED;
			glm::quat qy = glm::angleAxis(-delta.y, glm::vec3(0, 1, 0));
			glm::quat qx = glm::angleAxis(delta.x, m_right0);
			glm::quat q = qy * qx;
			m_eye = glm::rotate(q, m_eye0);
			m_center = glm::rotate(q, m_center0);
			m_up = glm::rotate(q, m_up0);
		}
	}

	void Camera::OnMouseButtonPressed(const Event::MouseButtonPressed& e)
	{
		switch (e.button) {
		case GLFW_MOUSE_BUTTON_RIGHT:
		case GLFW_MOUSE_BUTTON_MIDDLE:
			{
				m_mousePos0 = m_mousePos1 = Input::MousePos();
				m_eye0 = m_eye;
				m_center0 = m_center;
				m_up0 = m_up;
				m_right0 = m_right;
				break;
			}
		}
	}

	void Camera::OnMouseScrolled(const Event::MouseScrolled& e)
	{
		//m_view = glm::normalize(m_center - m_eye);
		glm::vec3 trans = ZOOM_SPEED * e.delta * m_view;
		m_eye += trans;
		m_center += trans;
	}
}
