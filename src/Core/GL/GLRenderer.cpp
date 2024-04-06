#include "mmpch.hpp"
#include "GLRenderer.hpp"

#include "GLShader.hpp"
#include "GLTexture.hpp"
#include "GLVertexArray.hpp"
#include "GLFrameBuffer.hpp"

#include "Core/MM/Camera/Camera.hpp"

namespace mm
{
	GLRenderer GLRenderer::s_instance;

	void GLRenderer::Init()
	{
		// Initialize UBOs and SSBOs
		m_materialUBO = std::make_unique<GLBuffer>(GL_UNIFORM_BUFFER);
		m_materialUBO->SetBase(MATERIAL_BASE);
		m_materialUBO->SetData(sizeof(MaterialUBOLayout), nullptr);

		m_cameraUBO = std::make_unique<GLBuffer>(GL_UNIFORM_BUFFER);
		m_cameraUBO->SetBase(CAMERA_BASE);
		m_cameraUBO->SetData(sizeof(CameraUBOLayout), nullptr);
	}

	void GLRenderer::SetMaterial(const MaterialUBOLayout& material)
	{
		m_materialUBO->SetSubData(0, sizeof(MaterialUBOLayout), (void*)&material);
	}

	void GLRenderer::SetCamera(const Camera& camera)
	{
		CameraUBOLayout ubo = {};
		ubo.proj = camera.GetProj();
		ubo.view = camera.GetView();
		ubo.viewProj = ubo.proj * ubo.view;

		m_cameraUBO->SetSubData(0, sizeof(CameraUBOLayout), &ubo);
	}

	void GLRenderer::BeginShader(GLShader* shader)
	{
		if (shader != nullptr) {
			shader->Use();
		}
		else {
			glUseProgram(0);
		}

		m_shader = shader;
	}

	void GLRenderer::EndShader()
	{
		BeginShader(nullptr);
	}

	void GLRenderer::BeginFramebuffer(GLFrameBuffer* framebuffer)
	{
		if (framebuffer != nullptr)
			framebuffer->Bind();
		else
			glBindFramebuffer(GL_FRAMEBUFFER, 0);

		m_framebuffer = framebuffer;
	}

	void GLRenderer::EndFramebuffer()
	{
		BeginFramebuffer(nullptr);
	}

	void GLRenderer::BeginPass(const GLPass& pass)
	{
		m_backup.blend = glIsEnabled(GL_BLEND);
		m_backup.depthTest = glIsEnabled(GL_DEPTH_TEST);
		m_backup.cullFace = glIsEnabled(GL_CULL_FACE);
		m_backup.shader = GetShader();

		SetEnable(GL_BLEND, pass.blend);
		SetEnable(GL_DEPTH_TEST, pass.depthTest);
		SetEnable(GL_CULL_FACE, pass.cullFace);
		BeginShader(pass.shader);
	}

	void GLRenderer::EndPass()
	{
		SetEnable(GL_BLEND, m_backup.blend);
		SetEnable(GL_DEPTH_TEST, m_backup.depthTest);
		SetEnable(GL_CULL_FACE, m_backup.cullFace);
		BeginShader(m_backup.shader);
	}

	void GLRenderer::SetEnable(uint32_t cap, bool enable)
	{
		if (enable)
			glEnable(cap);
		else
			glDisable(cap);
	}
}
