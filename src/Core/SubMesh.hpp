#pragma once

#include "Renderable.hpp"
#include "NamedObject.hpp"
#include "Graphics/VertexArray.hpp"

namespace mm
{

class Mesh;

class SubMesh : public Renderable, public NamedObject
{ 
public:
	SubMesh(StringView name, 
			Ref<VertexArray> va, Ref<Material> mat,
			u32 indexCount, u32 indexBegin = 0) : 
		Renderable{ mat },
		NamedObject{ name },
		m_vertexArray{ va },
		m_indexCount{ indexCount },
		m_indexBegin{ indexBegin } {}
	
	void Render() override;
	const VertexArray& GetVertexArray() const { return *m_vertexArray; }
	u32 GetIndexBegin() const { return m_indexBegin; }
	u32 GetIndexCount() const { return m_indexCount; }

private:
	Ref<VertexArray> m_vertexArray;
	u32 m_indexCount{};
	u32 m_indexBegin{};
};

}

