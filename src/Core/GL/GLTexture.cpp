#include "mmpch.hpp"
#include "GLTexture.hpp"

//TODO: Tex param

namespace mm
{
	GLTexture::GLTexture(const std::filesystem::path path, uint32_t target) :
		m_id(0),
		m_target(target)
	{
		glCreateTextures(m_target, 1, &m_id);

		int32_t x, y, ch;
		void* img = stbi_load(path.u8string().c_str(), &x, &y, &ch, 4);
		MM_ASSERT(img);
		MM_ASSERT(ch >= 3);

		m_size = glm::vec2(x, y);

		glTextureStorage2D(m_id, 1, GL_RGBA8, x, y);
		if (ch == 4) 
			glTextureSubImage2D(m_id, 0, 0, 0, x, y, GL_RGBA, GL_UNSIGNED_BYTE, img);
		else 
			glTextureSubImage2D(m_id, 0, 0, 0, x, y, GL_RGB, GL_UNSIGNED_BYTE, img);

		stbi_image_free(img);
		MM_INFO("id={0} path={1}: texture loaded: size={2}x{3}", 
			m_id, path.u8string().c_str(), x, y);
	}

	void GLTexture::Bind(uint32_t unit)
	{
		glBindTextureUnit(unit, m_id);
	}

	void GLTexture::SetWrap(uint32_t wrapS, uint32_t wrapT)
	{
		glTextureParameteri(m_id, GL_TEXTURE_WRAP_S, wrapS);
		glTextureParameteri(m_id, GL_TEXTURE_WRAP_T, wrapT);
	}

	void GLTexture::SetFilter(uint32_t min, uint32_t mag)
	{
		glTextureParameteri(m_id, GL_TEXTURE_MIN_FILTER, min);
		glTextureParameteri(m_id, GL_TEXTURE_MAG_FILTER, mag);
	}

	GLTexture::~GLTexture() 
	{
		glDeleteTextures(1, &m_id);
		MM_INFO("id={0}: texture unloaded", m_id);
	}
}
