#pragma once

#include "Renderable.hpp"

namespace mm
{

class SceneManager
{
public:
	Node& GetRootNode() { return *m_rootNode; }

	template <typename T, typename... Args>
	Renderable& AddRenderable(Args&&... args) {
		m_renderables.push_back(MakeScoped<T>(std::forward(args)...));
		return *m_renderables.back();
	}

private:

private:
	DynArray<Scoped<Renderable>> m_renderables{};
	Scoped<Node> m_rootNode{};
};

}

