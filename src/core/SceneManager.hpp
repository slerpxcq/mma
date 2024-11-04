#pragma once

#include "Renderable.hpp"
#include "Node.hpp"

namespace mm
{

class SceneManager
{
public:
	SceneManager() : m_rootNode(MakeScoped<Node>("Root")) {}

	Node& GetRootNode() { return *m_rootNode; }
	void AttachRenderable(Ref<Renderable> r) {
		m_renderables.push_back(r);
	}

	/* BEGIN TEST CODE */
	void Update(f32 deltaTime);
	void Render();
	/* END TEST CODE */

private:

private:
	const Scoped<Node> m_rootNode{};
	DynArray<Ref<Renderable>> m_renderables{};
};

}

