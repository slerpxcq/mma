#include "CorePch.hpp"
#include "Application.hpp"

#include "Globals.hpp"
#include "InputManager.hpp"
#include "../SceneManager.hpp"

#include "File/Text.hpp"
#include "File/Image.hpp"

#include "Common/Clock.hpp"

#include "Layer/CoreLayer.hpp"

#include "Graphics/OpenGL/Graphics_GL.hpp"
#include "Graphics/FrameBuffer.hpp"
#include "Graphics/Shader.hpp"
#include "Graphics/Program.hpp"
#include "Graphics/Texture2D.hpp"

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
	SetAppLogger(new Logger{ "App" });
	SetInputManager(new InputManager{});
	// SetRootNode(new Node{ "ROOT" });
	SetSceneManager(new SceneManager{});

	SetMainFrameBuffer(new FrameBuffer{ 1024, 1024,
					   { { Graphics::AttachmentType::COLOR, 0, Graphics::TexFormat::RGBA8 },
					     { Graphics::AttachmentType::DEPTH, 0, Graphics::TexFormat::D24S8 } } });

	FrameBuffer* fb = GetMainFrameBuffer();
	if (!fb->IsComplete()) {
		throw RuntimeError("Could not create main frame buffer");
	}

	// Load shader
	auto vsSrc = Text::Load("../../resources/shaders/test.vert");
	auto fsSrc = Text::Load("../../resources/shaders/test.frag");
	auto vs = MakeRef<Shader>(vsSrc->GetString(), Graphics::ShaderType::VERTEX);
	auto fs = MakeRef<Shader>(fsSrc->GetString(), Graphics::ShaderType::FRAGMENT);
	SetDefaultProgram(Ref<Program>(new Program{ vs, fs }));

	// Load default textures 
	for (i32 i = 0; i <= 10; ++i) {
		std::stringstream ss{};
		const char* path = "../../resources/textures/";
		ss << path << "toon" << std::setw(2) << std::setfill('0') << i << ".bmp";
		auto img = Image::Load(ss.str());
		auto tex = MakeRef<Texture2D>(img->GetWidth(), img->GetHeight(), Graphics::TexFormat::RGBA8);
		tex->SetSubImage(img->GetPixels(), Graphics::PixelType::UBYTE,
						 img->GetWidth(), img->GetHeight());
		GetDefaultTextures().push_back(tex);
	}

	RegisterCallbacks();

	m_layerStack.EmplaceBack<CoreLayer>();

	MM_CORE_INFO("Application started");
}

void Application::Shutdown()
{
	MM_CORE_INFO("Application shutting down...");

	GetDefaultTextures().clear();
	GetDefaultProgram().reset();

	delete GetMainFrameBuffer();
	// delete GetRootNode();
	delete GetSceneManager();
	delete GetInputManager();
	delete GetCoreLogger();
}

void Application::RegisterCallbacks()
{
	m_listener = MakeScoped<EventBus::Listener>(GetInputManager()->GetEventBus());
	m_listener->Listen(&Application::OnWindowClosed, this);
}

}
