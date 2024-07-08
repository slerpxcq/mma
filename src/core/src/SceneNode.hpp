#pragma once

#include "Node.hpp"

#include "CameraNode.hpp"

namespace mm
{

class SceneNode : public Node
{
public:
	SceneNode(const std::string& name = "") :
		Node(name, nullptr) {}

	void SetActiveCamera(std::shared_ptr<CameraNode> cam) {
		m_activeCamera = cam;
	}

	std::shared_ptr<CameraNode> GetActiveCamera() {
		return m_activeCamera.lock();
	}

	static std::shared_ptr<SceneNode> CreateDefault(const std::string& name);

private:
	std::weak_ptr<CameraNode> m_activeCamera;
};

}

