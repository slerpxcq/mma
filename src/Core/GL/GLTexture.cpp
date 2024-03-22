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

		if (ch == 3) {
			glTexImage2D(m_target, 0, GL_RGB, x, y, 0, GL_RGB, GL_UNSIGNED_BYTE, img);
		}
		else {
			glTexImage2D(m_target, 0, GL_RGBA, x, y, 0, GL_RGBA, GL_UNSIGNED_BYTE, img);
		}

		glTexParameteri(m_target, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(m_target, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(m_target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(m_target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(img);
		MM_INFO("Texture loaded: {0}", path.u8string().c_str());
	}

	void GLTexture::Bind(uint32_t active)
	{
		glActiveTexture(active);
		glBindTexture(m_target, m_id);
	}
}
