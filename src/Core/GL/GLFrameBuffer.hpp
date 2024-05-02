#pragma once

#include "GLTexture.hpp"
#include "GLTexture2D.hpp"

namespace mm
{
	class GLFrameBuffer
	{
	public:
		struct Attachment {
			uint32_t target;
			/* GL_TEXTURE_2D or GL_TEXTURE_2D_MULTISAMPLE */
			uint32_t type;
			uint32_t format;
			std::unique_ptr<GLTexture> texture;
		};

	public:
		GLFrameBuffer(glm::uvec2 size);
		~GLFrameBuffer();

		uint32_t GetId() const { return m_id; }

		//void AddAttachment(const std::string& name, uint32_t type, uint32_t target, uint32_t format);


		glm::uvec2 GetSize() const { return m_size; }
		//void AddAttachment(uint32_t attachment, uint32_t format);
		void Reload(glm::uvec2 size);
		void Bind();
		//void BindTarget(const std::string& name, uint32_t unit) { m_attachments[name]->Bind(unit); }

		uint32_t GetColorTarget() const { return m_colorTarget->GetId(); }
		void BindColorTarget(uint32_t unit) const { m_colorTarget->Bind(unit); }
		//uint32_t GetColorTarget() { return m_attachments["Color0"].texture->GetId(); }
		//void BindColorTarget(uint32_t unit) { m_attachments["Color0"].texture->Bind(unit); }

	private:
		uint32_t m_id = 0;
		//std::unordered_map<std::string, Attachment> m_attachments;
		//uint32_t m_colorTarget = 0;
		//uint32_t m_depthTarget = 0;
		std::unique_ptr<GLTexture2D> m_colorTarget;
		std::unique_ptr<GLTexture2D> m_depthTarget;
		glm::uvec2 m_size;
	};
}

