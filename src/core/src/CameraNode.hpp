#pragma once

#include "Node.hpp"

#include <renderer/src/GL/FrameBuffer.hpp>

#include <glm/gtc/matrix_transform.hpp>

namespace mm
{

class CameraNode : public Node
{
public:
	CameraNode(const std::string& name = "", 
			   std::shared_ptr<Node> parent = nullptr) :
		Node(name, parent) {}

	void SetPlane(float nearPlane, float farPlane) {
		m_near = nearPlane;
		m_far = farPlane;
	}

	void SetFov(float fov) { m_fovy = fov; }
	void SetAspect(float aspect) { m_aspect = aspect; }

	/* TODO: Camera needs information from framebuffer */
	glm::mat4 GetProjectionMatrix() const {
		return glm::perspective(m_fovy, m_aspect, m_near, m_far);
	}

	glm::mat4 GetViewMatrix() const {
		return Transform::toMat4(m_worldTransform.inverse());
	}

private:
	float m_aspect = 1.f;
	float m_near = 0.1f;
	float m_far = 100.f;
	float m_fovy = glm::radians(45.f);
};

}

