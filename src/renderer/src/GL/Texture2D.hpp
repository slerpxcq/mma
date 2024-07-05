#pragma once

#include "Texture.hpp"

#include <glm/glm.hpp>

namespace mm
{

class Texture2D : public Texture
{
public:
	Texture2D(glm::uvec2 size, uint32_t format, bool createHandle = true);
	Texture2D(Texture2D&&) noexcept;

	void SubImage(uint32_t level, 
				  uint32_t xoffset, uint32_t yoffset, 
				  uint32_t width, uint32_t height, 
				  uint32_t format, uint32_t type, const void* pixels);

	void Resize(glm::uvec2 size);
	glm::uvec2 GetSize() const { return m_size; }

private:
	glm::uvec2 m_size;
	uint32_t m_format;
};

}

