#include "CorePch.hpp"
#include "SceneManager.hpp"

namespace mm
{

void SceneManager::Update(f32 deltaTime)
{
	m_rootNode->UpdateSubtreeWorldTransform();
}

void SceneManager::Render()
{
	auto renderer = GetRenderer();
	renderer->BeginFrame();
	for (auto& r : m_renderables) {
		r->Render();
	}
	renderer->EndFrame();
}

}
