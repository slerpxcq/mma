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

	static void ErrorCallback(
		GLenum source,
		GLenum type,
		GLuint id,
		GLenum severity,
		GLsizei length,
		const GLchar* message,
		const void* userParam) 
	{
		if (type == GL_DEBUG_TYPE_ERROR) {
			MM_ERROR("GL error:\nWhat: {0}", message);
			//MM_ASSERT(0);
		}
	}

	void GLContext::RegisterErrorCallback()
	{
		glEnable(GL_DEBUG_OUTPUT);
		glDebugMessageCallback(ErrorCallback, nullptr);
	}

	void GLContext::SwapBuffers()
	{
		glfwSwapBuffers(m_window);
	}
}

