#pragma once

#include "Renderable.hpp"
#include "Node.hpp"

namespace mm
{

class SceneManager
{
public:
	SceneManager() : m_rootNode(MakeScoped<Node>("Root")) {
	}

	Node& GetRootNode() { return *m_rootNode; }

	template <typename T, typename... Args>
	Renderable& AddRenderable(Args&&... args) {
		m_renderables.push_back(MakeScoped<T>(std::forward(args)...));
		return *m_renderables.back();
	}

private:

private:
	const Scoped<Node> m_rootNode{};
	DynArray<Scoped<Renderable>> m_renderables{};
};

}

