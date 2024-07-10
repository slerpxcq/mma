#include "EditorPch.hpp"
#include "EditorApplication.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <renderer/src/Renderer.hpp>
#include <core/src/PMXFile.hpp>
#include <core/src/ModelLoader.hpp>

namespace mm
{

void EditorApplication::NewFrame(float deltaTime)
{
	for (auto panel : m_panels) {
		panel->OnUpdate();
	}

	m_sceneNode->UpdateWorldTransform();
	Renderer::Instance().RenderScene(m_sceneNode);

	m_ImGuiContext->Begin();

	for (auto panel : m_panels) {
		panel->OnUIRender();
	}

	m_ImGuiContext->End();
}

void EditorApplication::Startup()
{
	Application::Startup();
	RegisterWindowCallbacks();
	RegisterGLErrorCallback();

	m_eventBus = std::make_shared<dexode::EventBus>();
	m_ImGuiContext = std::make_unique<ImGuiContext>(m_window);

	auto menuBar = std::make_shared<MenuBar>();
	auto viewport = std::make_shared<ViewportPanel>();
	auto sceneHierarchy = std::make_shared<SceneHierarchyPanel>();

	m_panels.push_back(menuBar);
	m_panels.push_back(viewport);
	m_panels.push_back(sceneHierarchy);

	m_sceneNode = SceneNode::CreateDefault("Unnamed scene");
	sceneHierarchy->SetScene(m_sceneNode);

	auto cam = std::make_shared<CameraNode>("Main camera");
	cam->SetLocalTranslation(glm::vec3(0, 10, 50));
	m_sceneNode->AddChild(cam);
	m_sceneNode->SetActiveCamera(cam);
	viewport->SetCamera(cam);

	std::shared_ptr<PMXFile> pmx = std::make_shared<PMXFile>(u8"../../resources/model/つみ式ミクさん/000 ミクさん.pmx");
	auto model = ModelLoader::LoadFromPMX(pmx);
	m_sceneNode->AddChild(model);
}

void EditorApplication::Shutdown()
{
	Application::Shutdown();
}

void EditorApplication::RegisterWindowCallbacks()
{
	glfwSetWindowUserPointer(m_window, this);
}

void EditorApplication::RegisterGLErrorCallback()
{
	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(
		[] (GLenum source,
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

}

