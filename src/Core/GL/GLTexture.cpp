#include "mmpch.hpp"
#include "GLTexture.hpp"

//TODO: Tex param

namespace mm
{
	GLTexture::GLTexture(const std::filesystem::path path, uint32_t target) :
		m_target(target)
	{
		glGenTextures(1, &m_id);
		glBindTexture(m_target, m_id);

		int32_t x, y, ch;
		void* img = stbi_load(path.u8string().c_str(), &x, &y, &ch, 0);
		MM_ASSERT(img);
		MM_ASSERT(ch >= 3);

		m_size = glm::vec2(x, y);

		if (ch == 3) 
			glTexImage2D(m_target, 0, GL_RGB, x, y, 0, GL_RGB, GL_UNSIGNED_BYTE, img);
		else 
			glTexImage2D(m_target, 0, GL_RGBA, x, y, 0, GL_RGBA, GL_UNSIGNED_BYTE, img);

		SetWrap(GL_CLAMP_TO_BORDER, GL_CLAMP_TO_BORDER);
		SetFilter(GL_LINEAR, GL_LINEAR);

		glGenerateMipmap(m_target);

		stbi_image_free(img);
		MM_INFO("Texture loaded: {0}\n  Channel: {1}\n  Size: {2}x{3}",
			path.u8string().c_str(),
			ch, x, y);
	}

	void GLTexture::Bind(uint32_t active)
	{
		glActiveTexture(active);
		glBindTexture(m_target, m_id);
	}

	void GLTexture::SetWrap(uint32_t wrapS, uint32_t wrapT)
	{
		glTexParameteri(m_target, GL_TEXTURE_WRAP_S, wrapS);
		glTexParameteri(m_target, GL_TEXTURE_WRAP_T, wrapT);
	}

	void GLTexture::SetFilter(uint32_t min, uint32_t mag)
	{
		glTexParameteri(m_target, GL_TEXTURE_MIN_FILTER, min);
		glTexParameteri(m_target, GL_TEXTURE_MAG_FILTER, mag);
	}
}
