#include "CorePch.hpp"
#include "Mesh.hpp"

#include "SceneManager.hpp"

namespace mm
{

void Mesh::AddSubMesh(StringView name, Ref<Material> mat, u32 begin, u32 count) 
{
	auto sm = GetSceneManager();
	m_subMeshes.push_back(MakeRef<SubMesh>(*this, name, m_vertexArray, mat, count, begin));
	sm->AttachRenderable(m_subMeshes.back());
}

}
