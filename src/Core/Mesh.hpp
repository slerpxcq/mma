#pragma once

#include "SubMesh.hpp"
#include "Graphics/VertexArray.hpp"

namespace mm
{
class Mesh : public NamedObject
{
public:
	Mesh(StringView name, 
		 Ref<VertexArray> va) : 
		NamedObject{ name },
		m_vertexArray{ va } {}

	const VertexArray& GetVertexArray() const { return *m_vertexArray; }
	void CreateSubMesh(StringView name, Ref<Material> mat, u32 begin, u32 count);

private:
	Ref<VertexArray> m_vertexArray{};
	DynArray<SubMesh*> m_subMeshes{};
};

}

