#include "CorePch.hpp"
#include "VertexArray.hpp"

namespace mm
{

VertexArray::VertexArray(Ref<VertexBuffer> vb, Ref<IndexBuffer> ib) :
	m_vertexBuffer{ vb }, m_indexBuffer{ ib } 
{
	auto gfx = GetGraphics();
	gfx->CreateVertexArray(*this);
	gfx->SetIndexBuffer(*this, *ib);
	gfx->SetVertexBuffer(*this, *vb);
	vb->GetLayout().ApplyTo(*this);
}

}
