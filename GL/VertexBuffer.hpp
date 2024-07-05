#pragma once

#include "Buffer.hpp"

namespace mm
{

class VertexBuffer : public Buffer
{
public:
	VertexBuffer() : Buffer(GL_ARRAY_BUFFER) {}
	void SetStride(uint32_t stride) { m_stride = stride; }
	uint32_t GetStride() const { return m_stride; }

private:
	uint32_t m_stride = 0;
};

}

