#include "mmpch.hpp"
#include "Input.hpp"

#include "Application.hpp"

namespace mm
{
	glm::vec2 Input::MousePos()
	{
		double x, y;
		glfwGetCursorPos(Application::Instance().GetWindow(), &x, &y);
		return glm::vec2((float)x, (float)y);
	}

	bool Input::MouseButtonPressed(int32_t button)
	{
		return glfwGetMouseButton(Application::Instance().GetWindow(), button) == GLFW_PRESS;
	}

	bool Input::KeyPressed(int32_t key)
	{
		return glfwGetKey(Application::Instance().GetWindow(), key) == GLFW_PRESS;
	}
}
