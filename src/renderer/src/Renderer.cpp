#include "RendererPch.hpp"
#include "Renderer.hpp"

#include "GPUResourceManager.hpp"

namespace mm
{

void Renderer::Startup()
{
	GPUResourceManager::Init();
	MM_INFO("Renderer started-up successfully.");
}

void Renderer::Shutdown()
{
	GPUResourceManager::DeInit();
	MM_INFO("Renderer shutted-down successfully.");
}

}

