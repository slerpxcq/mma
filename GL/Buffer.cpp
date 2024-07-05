#include "RendererPch.hpp"
#include "Buffer.hpp"

namespace mm
{

Buffer::Buffer(uint32_t target) :
	m_target(target)
{
	glCreateBuffers(1, &m_rendererID);
	MM_INFO("Buffer loaded successfully; id=%u", m_rendererID);
}

Buffer::~Buffer()
{
	if (m_rendererID > 0) {
		glDeleteBuffers(1, &m_rendererID);
		MM_INFO("Buffer unloaded successfully; id=%u", m_rendererID);
	}
}

void Buffer::Data(uint32_t size, void* data, uint32_t usage)
{
	glNamedBufferData(m_rendererID, size, data, usage);
}

void Buffer::SubData(uint32_t offset, uint32_t size, void* data)
{
	glNamedBufferSubData(m_rendererID, offset, size, data);
}

}

