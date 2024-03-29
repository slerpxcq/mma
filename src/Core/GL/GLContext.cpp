#include "mmpch.hpp"
#include "GLContext.hpp"

namespace mm
{
	GLContext::GLContext(GLFWwindow* window) :
		m_window(window) 
	{
		glfwMakeContextCurrent(m_window);
		glfwSwapInterval(1);
		gladLoadGL();
		RegisterErrorCallback();
	}

	void GLContext::RegisterErrorCallback()
	{
		glEnable(GL_DEBUG_OUTPUT);
		glDebugMessageCallback([](
			GLenum source,
			GLenum type,
			GLuint id,
			GLenum severity,
			GLsizei length,
			const GLchar* message,
			const void* userParam) {
				if (type == GL_DEBUG_TYPE_ERROR) {
					MM_ERROR("GL error:\nWhat: {0}", message);
				}
		}, nullptr);
	}

	void GLContext::Clear()
	{
		glClearColor(0.1f, 0.1f, 0.1f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void GLContext::Enable(uint32_t what)
	{
		glEnable(what);
	}

	void GLContext::Disable(uint32_t what)
	{
		glDisable(what);
	}

	void GLContext::SwapBuffers()
	{
		glfwSwapBuffers(m_window);
	}

	void GLContext::Viewport(uint32_t x, uint32_t y)
	{
		glViewport(0, 0, x, y);
	}
}

