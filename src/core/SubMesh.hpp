#pragma once

#include "Renderable.hpp"
#include "Graphics/VertexArray.hpp"

namespace mm
{

class Mesh;

class SubMesh : public Renderable
{ 
public:
	SubMesh(Mesh& parent, StringView name, 
			Ref<VertexArray> va, Ref<Material> mat,
			u32 indexCount, u32 indexBegin = 0) : 
		Renderable{ mat },
		m_parent{ parent },
		m_name{ name }, 
		m_vertexArray{ va },
		m_indexCount{ indexCount },
		m_indexBegin{ indexBegin } {}
	
	void Render(Renderer::Visitor& v) override { v.Render(*this); }

private:
	Mesh& m_parent;
	const String m_name{};
	Ref<VertexArray> m_vertexArray;
	u32 m_indexCount{};
	u32 m_indexBegin{};
};

}

