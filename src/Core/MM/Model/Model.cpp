#include "mmpch.hpp"
#include "Model.hpp"

#include "Core/GL/GLRenderer.hpp"
#include "../World.hpp"

namespace mm
{
	Model::Model(World& world, const std::filesystem::path& path) :
		m_pmxFile(std::make_unique<PMXFile>(path)),
		m_world(world)
	{
		m_armature = std::make_unique<Armature>(*this);
		m_skin = std::make_unique<Skin>(*this);
		m_morph = std::make_unique<Morph>(*this);
		m_animation = std::make_unique<Animation>(*this);

		m_skinningBuffer = std::make_unique<GLBuffer>(GL_SHADER_STORAGE_BUFFER);
		m_skinningBuffer->SetData(m_armature->GetBones().size() * sizeof(glm::mat2x4), nullptr);
		m_skinningBuffer->SetBase(1);

		m_vertexMorphBuffer = std::make_unique<GLBuffer>(GL_SHADER_STORAGE_BUFFER);
		m_vertexMorphBuffer->SetData(m_skin->GetVertexCount() * sizeof(Morph::VertexLayout), nullptr);
		m_vertexMorphBuffer->SetBase(Morph::MORPH_VERTEX_BASE);

		m_materialMorphBuffer.resize(m_skin->GetMeshes().size());

		m_physicsData = m_world.GetPhysicsWorld().LoadModel(*this);
	}

	Animation* Model::LoadAnimation(const std::filesystem::path& path)
	{
		m_animation.reset();
		m_animation = std::make_unique<Animation>(*this, path);
		return m_animation.get();
	}

	void Model::Render(GLRenderer& renderer)
	{
		m_morph->Render(renderer);
		m_skin->Render(renderer);
	}

	void Model::Update(float deltaTime)
	{
		m_armature->UpdatePose();
		//m_armature->CalcSkinning();
	}
}
