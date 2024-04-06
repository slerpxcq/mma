#include "mmpch.hpp"
#include "GLTexture.hpp"

namespace mm
{
	GLTexture::GLTexture(uint32_t target) :
		m_target(target)
	{
		glCreateTextures(m_target, 1, &m_id);
	}

	void GLTexture::Bind(uint32_t unit) const
	{
		glBindTextureUnit(unit, m_id);
	}

	void GLTexture::Parameteri(uint32_t name, uint32_t param) 
	{
		glTextureParameteri(m_id, name, param);
	}

	GLTexture::~GLTexture() 
	{
		glDeleteTextures(1, &m_id);
		MM_INFO("id={0}: texture unloaded", m_id);
	}
}
