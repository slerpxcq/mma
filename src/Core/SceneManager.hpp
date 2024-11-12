#pragma once

#include "Renderable.hpp"
#include "Camera.hpp"
#include "Node.hpp"

namespace mm
{

class SceneManager
{
public:
	SceneManager() : m_rootNode(MakeScoped<Node>("Root")) {}

	Node* GetRootNode() const { return m_rootNode.get(); }
	auto& GetObjects() const { return m_objects; }
	auto& GetRenderables() const { return m_renderables; }

	template <typename T, typename... Args> T* CreateRenderable(Args&&... args);
	template <typename T, typename... Args> T* CreateObject(Args&&... args);

	void Update(f32 deltaTime);
	void Render();

private:

private:
	const Scoped<Node> m_rootNode{};
	DynArray<Scoped<SceneObject>> m_objects{};
	DynArray<Scoped<Renderable>> m_renderables{};
};

template <typename T, typename... Args>
T* SceneManager::CreateRenderable(Args&&... args) 
{
	auto renderable = Scoped<T>(new T{ std::forward<Args>(args)... });
	m_renderables.push_back(std::move(renderable));
	return static_cast<T*>(m_renderables.back().get());
}

template <typename T, typename... Args>
T* SceneManager::CreateObject(Args&&... args) 
{
	auto obj = Scoped<T>(new T{ std::forward<Args>(args)... });
	m_objects.push_back(std::move(obj));
	return static_cast<T*>(m_objects.back().get());
}

}

