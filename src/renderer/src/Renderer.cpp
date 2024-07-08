#include "RendererPch.hpp"
#include "Renderer.hpp"

#include "GPUResourceManager.hpp"
#include "MMVertex.hpp"

namespace mm
{

void Renderer::Startup()
{
	GPUResourceManager::Init();
	MMVertex::Init();
	CreateGlobalUniformBuffer();

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
}

void Renderer::RenderScene(SceneNode& scene)
{
	auto cam = scene.GetActiveCamera();
	if (!cam) {
		MM_INFO("No active camera was set when trying to render scene \"{0}\"", scene.GetName());
		return;
	}

	/* Set camera uniforms */
	GlobalUniformBlock ub = {};
	ub.projection = Transform::toMat4(cam->GetProjectionMatrix());
	ub.view = Transform::toMat4(cam->GetWorldTransform());
	ub.viewProjection = ub.projection * ub.view;
	m_globalUniformBuffer->SubData(0, sizeof(GlobalUniformBlock), &ub);

	/* Build Rendereable lists */

	/* Start rendering */
}

}

