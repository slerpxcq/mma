#include "CorePch.hpp"
#include "Application.hpp"

#include "Globals.hpp"
#include "InputManager.hpp"

#include "Common/Clock.hpp"

#include "Layer/CoreLayer.hpp"

#include "Graphics/OpenGL/Graphics_GL.hpp"
#include "Graphics/FrameBuffer.hpp"

#include "Node.hpp"

namespace mm
{

Application::Application(i32 argc, char** argv) :
	m_argc{ argc }, m_argv{ argv }, 
	m_graphics{ []() {
		SetGraphics(new Graphics_GL{});
		return GetGraphics();
	}() }
{
}

i32 Application::Run()
{
	Startup();

	static TimePoint tp = Clock::Now();
	while (m_running) {
		f32 deltaTime = Clock::Delta(tp);
		OnUpdate(deltaTime);

		m_window.BeginFrame();
		OnNewFrame(deltaTime);
		m_window.EndFrame();

		GetInputManager()->GetEventBus().Process();
	}

	Shutdown();

	return 0;
}

void Application::Startup()
{
	SetCoreLogger(new Logger{ "Core" });
	SetInputManager(new InputManager{});
	SetRootNode(new Node{ "ROOT" });

	SetMainFrameBuffer(new FrameBuffer{ 1024, 1024 });
	FrameBuffer* fb = GetMainFrameBuffer();
	fb->AddAttachment(Graphics::Attachment::COLOR, 0, Graphics::TexFormat::RGBA8);
	fb->AddAttachment(Graphics::Attachment::DEPTH, 0, Graphics::TexFormat::D24S8);
	if (!fb->IsComplete()) {
		throw RuntimeError("Could not create main frame buffer");
	}

	RegisterCallbacks();

	m_layerStack.EmplaceBack<CoreLayer>();

	MM_CORE_INFO("Application started");
}

void Application::Shutdown()
{
	MM_CORE_INFO("Application shutting down...");

	delete GetMainFrameBuffer();
	delete GetRootNode();
	delete GetInputManager();
	delete GetCoreLogger();
}

void Application::RegisterCallbacks()
{
	m_listener = MakeScoped<EventBus::Listener>(GetInputManager()->GetEventBus());
	m_listener->Listen(&Application::OnWindowClosed, this);
}

}
