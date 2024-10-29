#pragma once

#include "Buffer.hpp"

namespace mm
{

class IndexBuffer : public Buffer
{
public:
	IndexBuffer(const void* data, u32 indexSize, u32 indexCount) : 
		Buffer{ Target::INDEX }, 
		m_indexSize{ indexSize }, 
		m_indexCount{ indexCount } {
		SetData(data, indexSize * indexCount);
	}

	u32 GetIndexSize() const { return m_indexSize; }
	u32 GetIndexCount() const { return m_indexCount; }

private:
	u32 m_indexSize{};
	u32 m_indexCount{};
};

}

