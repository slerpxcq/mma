#pragma once

//#define MM_RENDERER_CMD_DEBUG

#ifdef MM_RENDERER_CMD_DEBUG
#define RENDERER_CMD(x) MM_WRAP(x; MM_INFO("{0}", #x))
#else
#define RENDERER_CMD(x) MM_WRAP(x)
#endif

namespace mm
{
	class GLShader;
	class GLContext;
	class GLTexture;
	class GLVertexArray;
	class GLFrameBuffer;

	class Camera;

	class GLRenderer
	{
	public:
		GLRenderer(GLContext& context);
		void SetCamera(const Camera& camera) { m_camera = &camera; }
		const Camera* GetCamera() { return m_camera; }

		// nullptr for default shader
		void BeginShader(GLShader* shader);
		void EndShader();
		// nullptr for default framebuffer
		void BeginFramebuffer(GLFrameBuffer* framebuffer);
		void EndFramebuffer();

		void BeginVertexArray(GLVertexArray*);
		void EndVertexArray();

		GLShader* GetShader() const { return m_shader; }
		void Enable(uint32_t what);
		void Disable(uint32_t what);
		void Viewport(uint32_t x, uint32_t y);
		void BlendFunc(uint32_t src, uint32_t dst);
		void BindTexture(const GLTexture& texture, uint32_t slot);
		void Draw(bool indexed, uint32_t mode, uint32_t offset, uint32_t count);
		void Barrier(uint32_t bitmask);
		void CullFace(uint32_t);
		void FrontFace(uint32_t);

		void Clear(const glm::vec4& color, uint32_t bitmask);

		GLContext& GetContext() { return m_context; }

	private:
		GLContext& m_context;
		const Camera* m_camera = nullptr;
		GLShader* m_shader = nullptr;
		GLFrameBuffer* m_framebuffer = nullptr;
		GLVertexArray* m_vertexArray = nullptr;
	};
}

