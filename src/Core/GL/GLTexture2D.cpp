#include "mmpch.hpp"
#include "GLTexture2D.hpp"

namespace mm
{
	GLTexture2D::GLTexture2D(const std::filesystem::path& path)  :
		GLTexture(GL_TEXTURE_2D)
	{
		int32_t x, y, ch;
		void* img = stbi_load(path.u8string().c_str(), &x, &y, &ch, 4);
		MM_ASSERT(img);
		MM_ASSERT(ch >= 3);

		m_size = glm::uvec2(x, y);

		glTextureStorage2D(m_id, 1, GL_RGBA8, x, y);
		glTextureSubImage2D(m_id, 0, 0, 0, x, y, GL_RGBA, GL_UNSIGNED_BYTE, img);

		stbi_image_free(img);
		MM_INFO("id={0}, path={1}: texture loaded: size={2}x{3}", 
			m_id, path.u8string().c_str(), x, y);
	}

	GLTexture2D::GLTexture2D(glm::uvec2 size, uint32_t format) :
		GLTexture(GL_TEXTURE_2D),
		m_size(size),
		m_format(format)
	{
		glTextureStorage2D(m_id, 1, format, size.x, size.y);
	}
}
