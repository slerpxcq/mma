#pragma once

#include "SubMesh.hpp"
#include "Graphics/VertexArray.hpp"

namespace mm
{

class Mesh 
{
public:
	Mesh(StringView name, 
		 Ref<VertexArray> va) : 
		m_name{ name },
		m_vertexArray{ va } {}

	const VertexArray& GetVertexArray() const { return *m_vertexArray; }
	void AddSubMesh(StringView name, Ref<Material> mat, u32 begin, u32 count);

private:
	String m_name{};
	Ref<VertexArray> m_vertexArray{};
	DynArray<Ref<SubMesh>> m_subMeshes{};
};

}

