#pragma once

#include "Drawable.hpp"
#include "Graphics/VertexArray.hpp"

namespace mm
{

class Mesh : public Drawable
{
public:
	Mesh(Ref<VertexBuffer> vb, Ref<IndexBuffer> ib) :
		m_vertexBuffer{ vb }, m_indexBuffer{ ib },
		m_vertexArray{ *vb, *ib } {}

private:
	VertexArray m_vertexArray;
	Ref<VertexBuffer> m_vertexBuffer{};
	Ref<IndexBuffer> m_indexBuffer{};
};

}

