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

	class Camera;

	class GLRenderer
	{
	public:
		using Command = std::function<void()>;

	public:
		GLRenderer(GLContext& context);
		void SetProj(const glm::mat4& proj) {
			m_proj = proj;
		}

		void SetCamera(const Camera* camera) {
			m_camera = camera;
		}

		const Camera* GetCamera() const {
			return m_camera;
		}

		GLContext& GetContext() {
			return m_context;
		}

		void UseShader(const GLShader* shader); 

		void Submit(Command cmd);
		void Commit();

	private:
		GLContext& m_context;
		const GLShader* m_shader;
		const Camera* m_camera;
		std::vector<Command> m_cmdQueue;
		glm::mat4 m_proj;
	};
}

