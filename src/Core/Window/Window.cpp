#include "CorePch.hpp"
#include "Window.hpp"

#include "Application/Globals.hpp"

#include "WindowImpl_GLFW.hpp"
#include "../Graphics/Graphics.hpp"

namespace mm
{

Window::Window(const ConstructInfo& m_info)
{
	switch (GetGraphics()->GetAPI()) {
	case Config::API::GL4:
		m_impl = MakeScoped<WindowImpl_GLFW>(m_info);
		break;
	default:
		break;
		//MM_ASSERT(0 && "API not supported yet");
	}
}

}

