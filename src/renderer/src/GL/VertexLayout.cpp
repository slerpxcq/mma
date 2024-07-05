#include "RendererPch.hpp"
#include "VertexLayout.hpp"

namespace mm
{

void VertexLayout::Push(const Attribute& attribute)
{
	m_attributes.push_back(attribute);
}

}
