#include "CorePch.hpp"
#include "Application.hpp"

#include "Globals.hpp"
#include "InputManager.hpp"
#include "../SceneManager.hpp"
#include "../Renderer.hpp"
#include "../Viewport.hpp"
#include "../Camera.hpp"
#include "../Grid.hpp"

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
	SetSceneManager(new SceneManager{});
	SetRenderer(new Renderer{});
	auto rdr = GetRenderer();
	rdr->LoadInternalTextures();
	rdr->LoadDefaultProgram();

	auto sm = GetSceneManager();
	auto cameraNode = sm->GetRootNode()->AddChild("camera_node");
	// auto camera = MakeRef<Camera>("camera");
	auto camera = sm->CreateObject<Camera>("camera");
	cameraNode->AttachObject(camera);
	cameraNode->SetWorldTranslation({ 0, 10, 20 });
	auto fb = Ref<FrameBuffer>{ new FrameBuffer{1024, 1024,
					   { { Graphics::AttachmentType::COLOR, 0, Graphics::TexFormat::RGBA8 },
						 { Graphics::AttachmentType::DEPTH, 0, Graphics::TexFormat::D24S8 } }} };
	SetMainViewport(new Viewport{ camera, fb });

	// Load grid shader 
	auto gridVsSrc = Text("../../resources/shaders/grid.vert");
	auto gridFsSrc = Text("../../resources/shaders/grid.frag");
	auto gridVs = MakeRef<Shader>(gridVsSrc.GetString(), Graphics::ShaderType::VERTEX);
	auto gridFs = MakeRef<Shader>(gridFsSrc.GetString(), Graphics::ShaderType::FRAGMENT);
	auto gridProg = Ref<Program>(new Program{ gridVs, gridFs });
	auto gridMat = Ref<Material>(new Material{ "grid_material", {}, gridProg });
	sm->CreateRenderable<Grid>(gridMat);

	RegisterCallbacks();

	m_layerStack.EmplaceBack<CoreLayer>();

	MM_CORE_INFO("Application started");
}

void Application::Shutdown()
{
	MM_CORE_INFO("Application shutting down...");

	//GetDefaultTextures().clear();
	//GetDefaultProgram().reset();

	delete GetMainViewport();
	delete GetRenderer();
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
