#include "RendererPch.hpp"
#include "Renderer.hpp"

#include "GPUResourceManager.hpp"
#include "MMVertex.hpp"

#include <core/src/RenderableNode.hpp>

#include <common/src/TextLoader.hpp>

namespace mm
{

void Renderer::Startup()
{
	GPUResourceManager::Init();
	MMVertex::Init();
	CreateGlobalUniformBuffer();
	LoadDefaultShader();

	MM_INFO("Renderer started-up successfully.");
}

void Renderer::Shutdown()
{
	MMVertex::DeInit();
	GPUResourceManager::DeInit();
	MM_INFO("Renderer shutted-down successfully.");
}

void Renderer::CreateGlobalUniformBuffer()
{
	m_globalUniformBuffer = GPUResourceManager::Instance().Load<Buffer>("Global uniform buffer", GL_UNIFORM_BUFFER);
	m_globalUniformBuffer->Data(sizeof(GlobalUniformBlock), nullptr);
	m_globalUniformBuffer->BindBase(0);
}

void Renderer::LoadDefaultShader()
{
	const char* vsPath = "../../resources/shaders/default.vert";
	const char* fsPath = "../../resources/shaders/default.frag";

	m_defaultShader = GPUResourceManager::Instance().Load<Shader>("Default shader");
	m_defaultShader->Compile(TextLoader::Load(vsPath), GL_VERTEX_SHADER);
	m_defaultShader->Compile(TextLoader::Load(fsPath), GL_FRAGMENT_SHADER);
	m_defaultShader->Link();
}

void Renderer::RenderScene(std::shared_ptr<SceneNode> scene)
{
	auto cam = scene->GetActiveCamera();
	if (!cam) {
		MM_INFO("No active camera was set when trying to render scene \"{0}\"", scene->GetName());
		return;
	}

	/* Set camera uniforms */
	GlobalUniformBlock ub = {};
	ub.projection = cam->GetProjectionMatrix();
	ub.view = cam->GetViewMatrix();
	ub.viewProjection = ub.projection * ub.view;
	m_globalUniformBuffer->SubData(0, sizeof(GlobalUniformBlock), &ub);

	/* Build Rendereable lists */

	/* Start rendering */
	auto fb = GPUResourceManager::Instance().Get<FrameBuffer>("rt_viewport_0");
	MM_ASSERT(fb);
	int32_t backup;
	glGetIntegerv(GL_FRAMEBUFFER_BINDING, &backup);
	fb->Bind();
	glViewport(0, 0, 1000, 1000);
	fb->Clear(glm::vec4(.1, .1, .1, 1), GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	Visit(scene);
	glBindFramebuffer(GL_FRAMEBUFFER, backup);
}

/* TEMPORARY: build renderable list use priority queue later */
void Renderer::Visit(std::shared_ptr<Node> node)
{
	for (auto child : node->GetChildren()) {
		if (auto renderable = std::dynamic_pointer_cast<RenderableNode>(child); renderable) {
			renderable->Render();
		}
		Visit(child);
	}
}

}

