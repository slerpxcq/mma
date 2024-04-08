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

		bool depthTest = true;

		bool blend = true;
		uint32_t blendS = GL_SRC_ALPHA;
		uint32_t blendD = GL_ONE_MINUS_SRC_ALPHA;

		bool cull = true;
		uint32_t frontFace = GL_CCW;
		uint32_t cullFace = GL_BACK;

		bool scissorTest = false;
		bool stencilTest = false;
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

	public:
		void Init();

		void SetCamera(const Camera& camera);
		void SetMaterial(const MaterialUBOLayout& material);

		// nullptr for default shader
		void SetShader(GLShader* shader);
		// nullptr for default framebuffer
		void SetFramebuffer(GLFrameBuffer* framebuffer);

		void BeginPass(const GLPass& pass);
		void EndPass();

		GLShader* GetShader() const { return m_shader; }
		GLFrameBuffer* GetFrameBuffer() { return m_framebuffer; }

	public:
		void SetEnable(uint32_t cap, bool enable);
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

