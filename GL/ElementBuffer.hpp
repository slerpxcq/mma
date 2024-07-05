#pragma once

#include "Buffer.hpp"

namespace mm
{

class ElementBuffer : public Buffer
{
public:
	ElementBuffer() : Buffer(GL_ELEMENT_ARRAY_BUFFER) {}
	void SetIndexType(uint32_t type) { 
		m_indexType = type; 
		m_indexSize = ((type == GL_UNSIGNED_BYTE ? 1 :
						type == GL_UNSIGNED_SHORT ? 2 :
						type == GL_UNSIGNED_INT ? 4 : 0));
		MM_ASSERT(m_indexSize && "Unknown index type");
	}

	uint32_t GetIndexSize() const { return m_indexSize; }
	uint32_t GetIndexType() const { return m_indexType; }

private:
	uint32_t m_indexType = 0;
	uint32_t m_indexSize = 0;
};

}

