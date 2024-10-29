#include "CorePch.hpp"
#include "ModelNode.hpp"

#include "File/PMXFile.hpp"
#include "Graphics/DefaultVertexLayout.hpp"

#include <glm/gtc/type_ptr.hpp>

namespace mm
{

static Ref<Mesh> LoadMesh(const PMXFile& pmx)
{
	/* Load indices */
	Ref<IndexBuffer> ib =
		MakeRef<IndexBuffer>(pmx.faces.data(),
							 sizeof(pmx.faces[0]),
							 pmx.faces.size() * 3);

	/* Load vertices */
	DynArray<DefaultVertexLayout::Struct> vertices{ pmx.vertices.size() };
	for (i32 i = 0; i < vertices.size(); ++i) {
		auto& v = vertices[i];
		const auto& pv = pmx.vertices[i];

		v.position = glm::make_vec3(pv.position);
		v.normal = glm::make_vec3(pv.normal);
		v.texCoord = glm::make_vec2(pv.uv);

		switch (static_cast<PMXFile::BlendingType>(pv.blendingType)) {
		case PMXFile::BlendingType::BDEF1:
			v.bones[0] = pv.blending.bdef1.boneIndex;
			v.bones[1] = v.bones[2] = v.bones[3] = -1;
			v.weights[0] = 1.f;
			break;
		case PMXFile::BlendingType::BDEF2:
			v.bones[0] = pv.blending.bdef2.boneIndices[0];
			v.bones[1] = pv.blending.bdef2.boneIndices[1];
			v.bones[2] = v.bones[3] = -1;
			v.weights[0] = pv.blending.bdef2.weight;
			v.weights[1] = 1.f - pv.blending.bdef2.weight;
			break;
		case PMXFile::BlendingType::BDEF4:
			for (i32 j = 0; j < 4; ++j) 
				v.bones[j] = pv.blending.bdef4.boneIndices[j];
			for (i32 j = 0; j < 3; ++j)
				v.weights[j] = pv.blending.bdef4.weights[j];
			break;
		case PMXFile::BlendingType::SDEF:
			v.bones[0] = pv.blending.sdef.boneIndices[0];
			v.bones[1] = pv.blending.sdef.boneIndices[1];
			v.bones[2] = -1; v.bones[3] = 0; /* SDEF mark */
			v.weights[0] = pv.blending.sdef.weight;
			v.weights[1] = 1.f - pv.blending.sdef.weight;
			v.sdef_c = glm::make_vec3(pv.blending.sdef.c);
			v.sdef_r0 = glm::make_vec3(pv.blending.sdef.r0);
			v.sdef_r1 = glm::make_vec3(pv.blending.sdef.r1);
			break;
		}
	}

	Ref<VertexBuffer> vb = 
		MakeRef<VertexBuffer>(vertices.data(),
							  DefaultVertexLayout::instance,
							  vertices.size());

	return MakeRef<Mesh>(vb, ib);
}

Scoped<ModelNode> ModelNode::LoadFromPMX(const PMXFile& pmx)
{
	auto model = MakeScoped<ModelNode>(pmx.info.nameJP);
	model->m_mesh = LoadMesh(pmx);

	/* TODO: Load other things */

	return std::move(model);
}

}
