#include "CorePch.hpp"
#include "Window.hpp"

#include "Application/Globals.hpp"

#include "WindowImpl_GLFW.hpp"

namespace mm
{

Window::Window(const ConstructInfo& info)
{
	switch (GetGraphics()->GetAPI()) {
	case Config::API::GL4:
		m_impl = MakeScoped<WindowImpl_GLFW>(info);
		break;
	default:
		break;
		//MM_ASSERT(0 && "API not supported yet");
	}
}

}

