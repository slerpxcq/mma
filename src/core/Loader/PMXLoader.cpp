#include "CorePch.hpp"
#include "PMXLoader.hpp"

namespace mm
{

Scoped<ModelNode> PMXLoader::Load(const PMXFile& pmx) 
{
	m_model = MakeScoped<ModelNode>(pmx.info.nameJP);

	LoadMesh(pmx);

	return std::move(m_model);
}

void PMXLoader::LoadMesh(const PMXFile& pmx)
{
	// TODO: Load indices
	 
	// TODO: Load vertices
}

}
