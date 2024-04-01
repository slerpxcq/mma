#include "mmpch.hpp"
#include "Model.hpp"

#include "Core/GL/GLRenderer.hpp"
#include "MMShader.hpp"
#include "World.hpp"

namespace mm
{
	std::unique_ptr<MMShader> Model::s_meshShader;
	std::unique_ptr<MMShader> Model::s_morphShader;

	Model::Model(World& world, const std::filesystem::path& path) :
		m_pmxFile(std::make_unique<PMXFile>(path)),
		m_world(world)
	{
		if (Model::s_meshShader == nullptr) {
			s_meshShader = std::make_unique<MMShader>();
			s_meshShader->Compile("resources/shaders/default.vert", GLShader::VERTEX);
			s_meshShader->Compile("resources/shaders/default.frag", GLShader::FRAGMENT);
			s_meshShader->Link();
		}

		if (Model::s_morphShader == nullptr) {
			s_morphShader = std::make_unique<MMShader>();
			s_morphShader->Compile("resources/shaders/morph.vert", GLShader::VERTEX);
			s_morphShader->Link();
		}

		m_armature = std::make_unique<Armature>(*this);
		m_skin = std::make_unique<Skin>(*this);
		m_morph = std::make_unique<Morph>(*this);

		m_skinningBuffer = std::make_unique<GLBuffer>(GL_SHADER_STORAGE_BUFFER);
		m_skinningBuffer->SetData(m_armature->GetBones().size() * sizeof(glm::mat4), nullptr);
		m_skinningBuffer->SetBase(MMShader::SKINNING_BASE);

		m_morphOffsetBuffer = std::make_unique<GLBuffer>(GL_SHADER_STORAGE_BUFFER);
		m_morphOffsetBuffer->SetData(m_skin->GetVertexCount() * sizeof(MMShader::MorphLayout), nullptr);
		m_morphOffsetBuffer->SetBase(MMShader::MORPH_BASE);

		// Physics
		m_physicsData = m_world.GetPhysicsWorld().LoadModel(*this);
	}

	void Model::Render(GLRenderer& renderer)
	{
		m_morph->GetWeights()[0] = 1.0f;
		m_morph->Render(renderer);
		m_skin->Render(renderer);
	}

	void Model::Update(float deltaTime)
	{
		m_armature->UpdatePose();
		//m_armature->CalcSkinning();
	}
}
