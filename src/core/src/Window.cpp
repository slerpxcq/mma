#include "CorePch.hpp"
#include "Window.hpp"

#include <WindowImpl_GLFW.hpp>

namespace mm
{

Window::Window(const ConstructInfo& info) :
	m_api{ info.api }
{
	switch (info.api) {
	case GraphicsAPI::GL4:
		m_impl = std::make_unique<WindowImpl_GLFW>(info);
		break;
	default:
		MM_ASSERT(0 && "Unknown graphics api");
	}
}

}

