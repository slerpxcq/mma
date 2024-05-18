#include "mmpch.hpp"
#include "Renderer.hpp"

#include "Core/MM/World/World.hpp"
#include "Core/GL/GLShader.hpp"
#include "Core/GL/GLTexture.hpp"
#include "Core/GL/GLVertexArray.hpp"
#include "Core/GL/GLFrameBuffer.hpp"

#include "Core/MM/Camera/Camera.hpp"

#include "../../ResourceManager/ResourceManager.hpp"

namespace mm
{
	Renderer Renderer::s_instance;

	void Renderer::Init()
	{
		// Initialize UBOs and SSBOs
		m_materialUBO = std::make_unique<GLBuffer>(GL_UNIFORM_BUFFER);
		m_materialUBO->SetBase(MATERIAL_BASE);
		m_materialUBO->SetData(sizeof(MaterialLayout), nullptr);

		m_cameraUBO = std::make_unique<GLBuffer>(GL_UNIFORM_BUFFER);
		m_cameraUBO->SetBase(CAMERA_BASE);
		m_cameraUBO->SetData(sizeof(CameraLayout), nullptr);

		m_lightUBO = std::make_unique<GLBuffer>(GL_UNIFORM_BUFFER);
		m_lightUBO->SetBase(LIGHT_BASE);
		m_lightUBO->SetData(sizeof(LightLayout), nullptr);
	}

	void Renderer::SetMaterial(const MaterialLayout& material)
	{
		m_materialUBO->SetSubData(0, sizeof(MaterialLayout), (void*)&material);
	}

	void Renderer::SetCamera(const Camera& camera)
	{
		CameraLayout ubo = {};
		ubo.proj = camera.GetProj();
		ubo.view = camera.GetView();
		ubo.viewProj = ubo.proj * ubo.view;

		m_cameraUBO->SetSubData(0, sizeof(CameraLayout), &ubo);
	}

	void Renderer::SetShader(GLShader* shader)
	{
		if (shader != nullptr) 
			shader->Use();
		else 
			glUseProgram(0);

		m_shader = shader;
	}

	void Renderer::SetFramebuffer(GLFrameBuffer* framebuffer)
	{
		if (framebuffer != nullptr)
			framebuffer->Bind();
		else
			glBindFramebuffer(GL_FRAMEBUFFER, 0);

		m_framebuffer = framebuffer;
	}

	void Renderer::BeginTechnique(const std::string& name)
	{
		m_activeTechnique = m_activeEffect->GetTechnique(name);
		MM_ASSERT(m_activeTechnique);
	}

	void Renderer::EndTechnique()
	{
		m_activeTechnique = nullptr;
	}

	static void SetEnable(uint32_t cap, bool enable)
	{
		if (enable)
			glEnable(cap);
		else
			glDisable(cap);
	}

	void Renderer::BeginPass(const Effect::Pass& pass)
	{
		m_activePass = &pass;
		/* Backup states */
		/* Depth test */
		m_backupState.depthTest = glIsEnabled(GL_DEPTH_TEST);

		/* Blend */
		m_backupState.blend = glIsEnabled(GL_BLEND);
		glGetIntegerv(GL_BLEND_SRC_ALPHA, (int32_t*)&m_backupState.blendSrc);
		glGetIntegerv(GL_BLEND_DST_ALPHA, (int32_t*)&m_backupState.blendDst);

		/* Cull face */
		m_backupState.cullFace = glIsEnabled(GL_CULL_FACE);

		/* Set states */
		SetEnable(GL_DEPTH_TEST, pass.depthTest);
		SetEnable(GL_BLEND, pass.blend);
		glBlendFunc(pass.blendSrc, pass.blendDst);
		SetEnable(GL_CULL_FACE, pass.cullFace);
		glFrontFace(pass.frontFace);

		/* Use shader */
		pass.program->Use();

		/* Set uniforms*/
		for (auto& u : pass.uniforms) {
			switch (u.type) {
			case Effect::Uniform::TYPE_INT:
				pass.program->Uniform(u.name, std::any_cast<int32_t>(u.value));
				break;
			case Effect::Uniform::TYPE_FLOAT:
				pass.program->Uniform(u.name, std::any_cast<float>(u.value));
				break;
			default:
				MM_ASSERT(0 && "Unknown uniform type");
			}
		}
	}

	void Renderer::EndPass()
	{
		glMemoryBarrier(GL_ALL_BARRIER_BITS);

		m_activePass = nullptr;
		SetEnable(GL_DEPTH_TEST, m_backupState.depthTest);
		SetEnable(GL_BLEND, m_backupState.blend);
		glBlendFunc(m_backupState.blendSrc, m_backupState.blendDst);
		SetEnable(GL_CULL_FACE, m_backupState.cullFace);
	}

	void Renderer::RenderMorph(const Model& model)
	{
		SetShader(ResourceManager::Instance().GetShader("morph"));
		model.m_vertexMorphBuffer->SetBase(Model::MORPH_VERTEX_SSBO_BASE);

		const auto& morph = *model.m_morph;

		glEnable(GL_RASTERIZER_DISCARD);

		for (const auto& target : morph.m_vertexTargets) {
			m_shader->Uniform("u_weight", 1, &morph.m_weights[target.index]);
			target.vertexArray->Bind();
			target.vertexArray->DrawArray(GL_POINTS, 0, target.offsetCount);
			glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
		}

		glDisable(GL_RASTERIZER_DISCARD);
	}

	GLTexture& Renderer::GetTexture(const Skin& skin, int32_t idx)
	{
		return (idx < 0) ? 
			*ResourceManager::Instance().GetTexture("toon00.bmp") :
			*skin.m_textures[idx];
	}

	GLTexture& Renderer::GetToon(const Skin& skin, const Mesh& mesh)
	{
		static constexpr const char* toonTable[] = {
			"toon01.bmp", "toon02.bmp",
			"toon03.bmp", "toon04.bmp",
			"toon05.bmp", "toon06.bmp",
			"toon07.bmp", "toon08.bmp",
			"toon09.bmp", "toon10.bmp",
		};

		return ((mesh.material.flags >> 8) & PMXFile::TOON_SHARED_BIT) ?
			*ResourceManager::Instance().GetTexture(toonTable[mesh.toonIndex]) :
			GetTexture(skin, mesh.toonIndex);
	}

	void Renderer::RenderSkin(const Model& model)
	{
		model.m_skinningBuffer->SetBase(Model::SKINNING_SSBO_BASE);
		model.m_vertexMorphBuffer->SetBase(Model::MORPH_VERTEX_SSBO_BASE);

		const auto& skin = *model.m_skin;

		/* Outline pass */
		for (auto& mesh : skin.m_meshes) {
			if (!(mesh.material.flags & PMXFile::MATERIAL_EDGE_BIT))
				continue;
			BeginEffect(mesh.effect);
			BeginTechnique("OutlineTec");
			for (auto& pass : GetActiveTechniquePasses()) {
				BeginPass(pass);
				skin.m_vertexArray->Bind();
				skin.m_vertexArray->DrawElem(GL_TRIANGLES, mesh.elemOffset, mesh.elemCount);
				EndPass();
			}
			EndTechnique();
			EndEffect();
		}

		/* Main pass */
		for (uint32_t meshIndex = 0; meshIndex < skin.m_meshes.size(); ++meshIndex) {
			auto& mesh = skin.m_meshes[meshIndex];

			BeginEffect(mesh.effect);
			BeginTechnique("MainTec");
			//renderer.BeginTechnique("PBRTec");

			for (const auto& pass : GetActiveTechniquePasses()) {
				BeginPass(pass);

				if (mesh.material.flags & PMXFile::MATERIAL_NO_CULL_BIT)
					glDisable(GL_CULL_FACE);

				/* These should be set in BeginPass */
				GLTexture& albedo = GetTexture(skin, mesh.albedoIndex);
				GLTexture& sph = GetTexture(skin, mesh.sphIndex);
				GLTexture& toon = GetToon(skin, mesh);
				GLTexture& skybox = *ResourceManager::Instance().GetTexture("skybox");
				albedo.Bind(0);
				sph.Bind(1);
				toon.Bind(2);
				skybox.Bind(3);

				//SetMaterial(model.m_materialMorphBuffer[meshIndex]);
				SetMaterial(mesh.material);

				GLShader* shader = pass.program;
				shader->Uniform("u_albedo", 0);
				shader->Uniform("u_sph", 1);
				shader->Uniform("u_toon", 2);
				shader->Uniform("u_skybox", 3);

				skin.m_vertexArray->Bind();
				skin.m_vertexArray->DrawElem(GL_TRIANGLES, mesh.elemOffset, mesh.elemCount);

				EndPass();
			}

			EndTechnique();
			EndEffect();
		}
	}

	void Renderer::RenderSceneForward(const World& world)
	{
		/* Setup camera */
		SetCamera(world.GetCamera());

		/* Setup lights */
		LightLayout l = {};
		l.color = glm::vec4(world.m_directionalLight.GetColor(), 0);
		l.direction = glm::vec4(world.m_directionalLight.GetDirection(), 0);
		SetLight(l);

		for (auto& model : world.GetModels()) {
			RenderMorph(*model);
			RenderSkin(*model);
		}
	}
}


