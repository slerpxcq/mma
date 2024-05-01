#pragma once

#include "Core/GL/GLBuffer.hpp"
#include "Effect.hpp"

namespace mm
{
	class GLShader;
	class GLContext;
	class GLTexture;
	class GLVertexArray;
	class GLFrameBuffer;

	class Camera;

	// std140
	struct MaterialLayout 
	{
		glm::vec4 diffuse;
		glm::vec4 specular;
		glm::vec4 ambient;
		glm::vec4 edge;
		float edgeSize;
		//                [23:16]                [15:8]                  [7:0]
		//mat.flags = (pm.toonFlag << 16) | (pm.sphereMode << 8) | (pm.drawFlag);
		uint32_t flags;
	};

	// std140
	struct CameraLayout 
	{
		glm::mat4 view;
		glm::mat4 proj;
		glm::mat4 viewProj;
	};

	class Renderer
	{
	public:
		static constexpr uint32_t MATERIAL_BASE = 0;
		static constexpr uint32_t CAMERA_BASE = 1;

	public:
		static Renderer& Instance() { return s_instance; }
		void Init();

		void SetCamera(const Camera& camera);
		void SetMaterial(const MaterialLayout& material);

		void SetShader(GLShader* shader);
		void SetFramebuffer(GLFrameBuffer* framebuffer);

		void BeginEffect(Effect* effect) { m_activeEffect = effect; }
		void BeginTechnique(const std::string& name);
		const auto& GetActiveTechniquePasses() { return m_activeTechnique->passes; };
		void BeginPass(const Effect::Pass& pass);
		void EndPass();
		void EndTechnique();
		void EndEffect() { m_activeEffect = nullptr; }

		GLShader* GetActiveShader() const { return m_shader; }
		GLFrameBuffer* GetActiveFrameBuffer() { return m_framebuffer; }

	private:
		Renderer() {};

	private:
		static Renderer s_instance;

	private:
		GLShader* m_shader = nullptr;
		GLFrameBuffer* m_framebuffer = nullptr;

		std::unique_ptr<GLBuffer> m_materialUBO;
		std::unique_ptr<GLBuffer> m_cameraUBO;

		Effect* m_activeEffect = nullptr;
		Effect::Technique* m_activeTechnique = nullptr;
		const Effect::Pass* m_activePass = nullptr;
		Effect::Pass m_backupState;
	};
}

