#pragma once

#include "Buffer.hpp"

namespace mm
{

class IndexBuffer : public Buffer
{
public:
	IndexBuffer(const void* data, Graphics::IndexType type, u32 count) : 
		Buffer{ Target::INDEX }, 
		m_type{ type }, 
		m_count{ count } {
		SetStorage(data, Graphics::ToSize(type) * count);
	}

	Graphics::IndexType GetIndexType() const { return m_type; }
	u32 GetIndexSize() const { return Graphics::ToSize(m_type); }
	u32 GetIndexCount() const { return m_count; }

private:
	Graphics::IndexType m_type{};
	u32 m_count{};
};

}

