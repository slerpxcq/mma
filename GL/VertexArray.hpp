#pragma once

#include "GPUResource.hpp"
#include "Buffer.hpp"
#include "ElementBuffer.hpp"
#include "VertexBuffer.hpp"
#include "VertexLayout.hpp"

namespace mm
{

class VertexArray : public GPUResource
{
public:
	VertexArray(const VertexBuffer& vbo, 
				const ElementBuffer& ebo,
				const VertexLayout& layout);

	~VertexArray();

	void DrawArrays(uint32_t mode, uint32_t first, uint32_t count) const;
	void DrawElements(uint32_t mode, uint32_t first, uint32_t count) const;

private:
	const ElementBuffer& m_elementBuffer;
	const VertexBuffer& m_vertexBuffer;
	const VertexLayout& m_layout;
};

}

