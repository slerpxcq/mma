#include "CorePch.hpp"
#include "Window.hpp"

#include "Globals.hpp"

#include "WindowImpl_GLFW.hpp"

namespace mm
{

Window::Window(const ConstructInfo& info)
{
	switch (GetGraphicsAPI()) {
	case Config::GraphicsAPI::GL4:
		m_impl = MakeScoped<WindowImpl_GLFW>(info);
		break;
	default:
		break;
		//MM_ASSERT(0 && "API not supported yet");
	}
}

}

