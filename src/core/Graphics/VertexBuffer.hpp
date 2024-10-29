#pragma once

#include "Buffer.hpp"
#include "VertexLayout.hpp"

namespace mm
{

class VertexBuffer : public Buffer
{
public:
	VertexBuffer(const void* data, 
				 const VertexLayout& layout, 
				 u32 count) : 
		Buffer{ Target::VERTEX },
		m_layout{ layout },
		m_vertexCount{ count } {
		SetData(data, count * layout.GetSize());
	}

	u32 GetVertexCount() const { return m_vertexCount; }

private:
	const VertexLayout& m_layout;
	u32 m_vertexCount{};
};

}

