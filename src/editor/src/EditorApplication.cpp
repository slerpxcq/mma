#include "EditorPch.hpp"
#include "EditorApplication.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <renderer/src/Renderer.hpp>
#include <core/src/PMXFile.hpp>

namespace mm
{

void EditorApplication::NewFrame(float deltaTime)
{
	m_sceneNode->UpdateWorldTransform();
	Renderer::Get().RenderScene(*m_sceneNode);

	m_GUIContext->Begin();

	m_viewport->OnUIRender();
	m_menuBar->OnUIRender();
	m_sceneHierarchy->OnUIRender();

	m_GUIContext->End();
}

void EditorApplication::Startup()
{
	Application::Startup();
	m_eventBus = std::make_shared<dexode::EventBus>();
	m_GUIContext = std::make_unique<ImGuiContext>(m_window);
	m_menuBar = std::make_unique<MenuBar>();
	m_viewport = std::make_unique<ViewportPanel>();

	m_sceneNode = SceneNode::CreateDefault("Unnamed scene");
	m_sceneHierarchy = std::make_unique<SceneHierarchyPanel>();
	m_sceneHierarchy->SetScene(m_sceneNode);

	auto cam = std::make_shared<CameraNode>("Main camera");
	m_sceneNode->AddChild(cam);
	m_sceneNode->SetActiveCamera(cam);
	m_viewport->SetCamera(cam);

	std::unique_ptr<PMXFile> pmx = std::make_unique<PMXFile>(u8"../../resources/model/つみ式ミクさん/000 ミクさん.pmx");
}

void EditorApplication::Shutdown()
{
	Application::Shutdown();
}

void EditorApplication::RegisterWindowCallbacks()
{
	glfwSetWindowUserPointer(m_window, this);
}

}

