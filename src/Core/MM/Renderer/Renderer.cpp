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
		m_cameraUBO->SetData(sizeof(CameraUBOLayout), nullptr);
	}

	void Renderer::SetMaterial(const MaterialLayout& material)
	{
		m_materialUBO->SetSubData(0, sizeof(MaterialLayout), (void*)&material);
	}

	void Renderer::SetCamera(const Camera& camera)
	{
		CameraUBOLayout ubo = {};
		ubo.proj = camera.GetProj();
		ubo.view = camera.GetView();
		ubo.viewProj = ubo.proj * ubo.view;

		m_cameraUBO->SetSubData(0, sizeof(CameraUBOLayout), &ubo);
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

		pass.program->Use();
	}

	void Renderer::EndPass()
	{
		SetEnable(GL_DEPTH_TEST, m_backupState.depthTest);
		SetEnable(GL_BLEND, m_backupState.blend);
		glBlendFunc(m_backupState.blendSrc, m_backupState.blendDst);
		SetEnable(GL_CULL_FACE, m_backupState.cullFace);
	}
}
