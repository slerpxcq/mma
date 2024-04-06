#pragma once

#include "Core/GL/GLBuffer.hpp"

namespace mm
{
	class GLShader;
	class GLContext;
	class GLTexture;
	class GLVertexArray;
	class GLFrameBuffer;

	class Camera;

	struct GLPass
	{
		GLShader* shader;
		//std::vector<GLTexture*> textures;

		int32_t depthTest;
		int32_t blend;
		int32_t cullFace;
	};

	// std140
	struct MaterialUBOLayout 
	{
		glm::vec4 diffuse;
		glm::vec4 specular;
		glm::vec4 ambient;
		glm::vec4 edge;
		float edgeSize;
		uint32_t flags;
	};

	// std140
	struct CameraUBOLayout 
	{
		glm::mat4 view;
		glm::mat4 proj;
		glm::mat4 viewProj;
	};

	class GLRenderer
	{
	public:
		static constexpr uint32_t MATERIAL_BASE = 0;
		static constexpr uint32_t CAMERA_BASE = 1;
		//static constexpr uint32_t SKINNING_BASE = 1;
		//static constexpr int32_t ALBEDO_TEX_UNIT = 0;
		//static constexpr int32_t SPH_TEX_UNIT = 1;
		//static constexpr int32_t TOON_TEX_UNIT = 2;

		//static constexpr uint32_t SPH_MODE_OFFSET = 8;
		//static constexpr uint32_t TOON_FLAG_OFFSET = 16;

	public:
		void Init();

		void SetCamera(const Camera& camera);
		void SetMaterial(const MaterialUBOLayout& material);

		// nullptr for default shader
		void BeginShader(GLShader* shader);
		void EndShader();
		// nullptr for default framebuffer
		void BeginFramebuffer(GLFrameBuffer* framebuffer);
		void EndFramebuffer();

		void BeginPass(const GLPass& pass);
		void EndPass();

		void SetEnable(uint32_t cap, bool enable);

		template <typename T>
		void Uniform(const std::string& name, uint32_t count, const T* v) { 
			m_shader->Uniform(name, count, v); 
		}

		GLShader* GetShader() const { return m_shader; }
		GLFrameBuffer* GetFrameBuffer() { return m_framebuffer; }

	public:
		static GLRenderer s_instance;

	private:
		GLRenderer() {};

	private:
		//const Camera* m_camera = nullptr;
		GLShader* m_shader = nullptr;
		GLFrameBuffer* m_framebuffer = nullptr;

		std::unique_ptr<GLBuffer> m_materialUBO;
		std::unique_ptr<GLBuffer> m_cameraUBO;
		//std::unique_ptr<GLBuffer> m_lightUBO;

		GLPass m_backup;
	};
}

