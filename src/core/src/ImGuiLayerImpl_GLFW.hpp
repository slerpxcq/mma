#pragma once

#include "ImGuiLayer.hpp"

struct GLFWwindow;

namespace mm
{

class ImGuiLayerImpl_GLFW : public ImGuiLayer::Impl
{
public:
	ImGuiLayerImpl_GLFW(GLFWwindow* window);
	virtual void Begin() override;
	virtual void End() override;

	~ImGuiLayerImpl_GLFW();

private:
	GLFWwindow* m_window;
};

}

