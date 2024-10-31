#pragma once

#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"

namespace mm
{

class VertexArray : public GPUResource
{
public:
	VertexArray(Ref<VertexBuffer> vb, Ref<IndexBuffer> ib);
	~VertexArray() { GetGraphics()->DeleteVertexArray(*this); }

private:
	Ref<VertexBuffer> m_vertexBuffer;
	Ref<IndexBuffer> m_indexBuffer;
};

}

