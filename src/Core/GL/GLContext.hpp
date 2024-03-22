#pragma once

struct GLFWwindow;

namespace mm
{
	class GLContext
	{
	public:
		GLContext(GLFWwindow* window);

		void SwapBuffers();
		void Viewport(uint32_t x, uint32_t y);
		void Clear();
		void RegisterErrorCallback();
	private:
		GLFWwindow* m_window;
	};
}


