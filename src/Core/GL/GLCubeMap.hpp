#pragma once

#include "GLTexture.hpp"

namespace mm
{
	struct GLCubeMapConstructInfo
	{
		std::filesystem::path paths[6];
	};

	class GLCubeMap : public GLTexture
	{
	public:
		GLCubeMap(const GLCubeMapConstructInfo& constructInfo);

	};
}

