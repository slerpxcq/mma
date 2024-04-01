#pragma once

struct GLFWwindow;

namespace mm
{
	class GLContext
	{
	public:
		GLContext(GLFWwindow* window);
		void SwapBuffers();
		void RegisterErrorCallback();

	private:
		GLFWwindow* m_window;
	};
}


