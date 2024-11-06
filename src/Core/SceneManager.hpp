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

	Node& GetRootNode() { return *m_rootNode; }

	template <typename T, typename... Args>
	T& AddRenderable(Args&&... args) {
		auto renderable = Ref<T>(new T{ std::forward<Args>(args)... });
		m_renderables.push_back(renderable);
		return *renderable;
	}

	template <typename T, typename... Args>
	T& AddCamera(Args&&... args) {
		auto camera = Ref<T>(new T{ std::forward<Args>(args)... });
		m_cameras.push_back(camera);
		return *camera;
	}

	void Update(f32 deltaTime);
	void Render();

private:

private:
	const Scoped<Node> m_rootNode{};
	DynArray<Ref<Renderable>> m_renderables{};
	DynArray<Ref<Camera>> m_cameras{};
};

}

