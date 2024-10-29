#pragma once

#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"

namespace mm
{

class VertexArray : public GPUResource
{
public:
	VertexArray(const VertexBuffer& vb, const IndexBuffer& ib) :
		m_vertexBuffer{ vb }, m_indexBuffer{ ib } {
		/* TODO: Setup vertex attributes */
	}

private:
	const VertexBuffer& m_vertexBuffer;
	const IndexBuffer& m_indexBuffer;
};

}

