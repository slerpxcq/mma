#include "mmpch.hpp"
#include "Renderer.hpp"

#include "Core/GL/GLShader.hpp"
#include "Core/GL/GLTexture.hpp"
#include "Core/GL/GLVertexArray.hpp"
#include "Core/GL/GLFrameBuffer.hpp"

#include "Core/MM/Camera/Camera.hpp"

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
			default:
				MM_ASSERT(0 && "Unknown uniform type");
			}
		}
	}

	void Renderer::EndPass()
	{
		m_activePass = nullptr;
		SetEnable(GL_DEPTH_TEST, m_backupState.depthTest);
		SetEnable(GL_BLEND, m_backupState.blend);
		glBlendFunc(m_backupState.blendSrc, m_backupState.blendDst);
		SetEnable(GL_CULL_FACE, m_backupState.cullFace);
	}
}
