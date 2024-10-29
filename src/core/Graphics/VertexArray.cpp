#include "CorePch.hpp"
#include "VertexArray.hpp"

namespace mm
{

VertexArray::VertexArray(const VertexBuffer& vb, const IndexBuffer& ib) :
	m_vertexBuffer{ vb }, m_indexBuffer{ ib } 
{
	const auto& gfx = GetGraphics();
	gfx->CreateVertexArray(*this);
	gfx->SetIndexBuffer(*this, ib);
	gfx->SetVertexBuffer(*this, vb);
	vb.GetLayout().ApplyTo(*this);
}

}
