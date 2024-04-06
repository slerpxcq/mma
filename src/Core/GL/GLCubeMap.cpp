#include "mmpch.hpp"
#include "GLCubeMap.hpp"

namespace mm
{
	GLCubeMap::GLCubeMap(const GLCubeMapConstructInfo& constructInfo) :
		GLTexture(GL_TEXTURE_CUBE_MAP)
	{
		glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &m_id);

		for (uint32_t i = 0; i < 6; ++i) {
			int32_t x, y, ch;
			void* img = stbi_load(constructInfo.paths[i].u8string().c_str(), &x, &y, &ch, 4);
			MM_ASSERT(img);

			if (i == 0) {
				glTextureStorage2D(m_id, 1, GL_RGBA8, x, y);
				Parameteri(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				Parameteri(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
				Parameteri(GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
			}

			glTextureSubImage3D(m_id, 0, 0, 0, i, x, y, 1, GL_RGBA, GL_UNSIGNED_BYTE, img);

			stbi_image_free(img);
			MM_INFO("id={0}, path={1}: cubemap face loaded: size={2}x{3}", 
				m_id, constructInfo.paths[i].u8string().c_str(), x, y);
		}
	}
}
