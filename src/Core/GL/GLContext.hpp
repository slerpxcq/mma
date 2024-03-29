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
		void Enable(uint32_t what);
		void Disable(uint32_t what);
		void RegisterErrorCallback();
	private:
		GLFWwindow* m_window;
	};
}


