#include "RendererPch.hpp"
#include "VertexArray.hpp"

#include <glad/gl.h>

namespace mm
{

VertexArray::VertexArray(const VertexBuffer& vbo, 
						 const ElementBuffer& ebo,
						 const VertexLayout& layout)
{
	glCreateVertexArrays(1, &m_rendererID);
	MM_INFO("Vertex array loaded successfully; id=%u", m_rendererID);
}

void VertexArray::SetElementBuffer(Ref<ElementBuffer> ebo)
{
	m_elementBuffer = ebo;
	uint32_t size = ebo.Get()->GetIndexSize();
	MM_ASSERT(size > 0 && "Index size must be greated than zero!");
	glVertexArrayElementBuffer(m_rendererID, ebo.Get()->GetID());
}

void VertexArray::SetVertexBuffer(Ref<VertexBuffer> vbo)
{
	m_vertexBuffer = vbo;
	uint32_t stride = vbo.Get()->GetStride();
	MM_ASSERT(stride > 0 && "Stride must be greater than zero!");
	glVertexArrayVertexBuffer(m_rendererID, 0, vbo.Get()->GetID(), 0, stride);
}

void VertexArray::SetLayout(const VertexLayout& layout)
{
	for (uint32_t location = 0, offset = 0; location < layout.m_attributes.size(); ++location) {
		const auto& attribute = layout.m_attributes[location];
		switch (attribute.type) {
		case GL_FLOAT:
			glVertexArrayAttribFormat(m_rendererID, location, attribute.count, GL_FLOAT, GL_FALSE, offset);
			break;
		case GL_INT:
			glVertexArrayAttribIFormat(m_rendererID, location, attribute.count, GL_INT, offset);
			break;
		default:
			MM_ASSERT(0 && "Unknown vertex attribute type.");
		}
		glEnableVertexArrayAttrib(m_rendererID, location);
		glVertexArrayAttribBinding(m_rendererID, location, 0);
		offset += 4 * attribute.count;
		MM_INFO("Vertex attribute found; type=%u, count=%u, location=%u, offset=%u", attribute.type, attribute.count, location, offset);
	}
}

void VertexArray::DrawArrays(uint32_t mode, uint32_t first, uint32_t count) const
{
	glBindVertexArray(m_rendererID);
	glDrawArrays(mode, first, count);
}

void VertexArray::DrawElements(uint32_t mode, uint32_t first, uint32_t count) const
{
	glBindVertexArray(m_rendererID);
	glDrawElements(mode, count, ->GetIndexType(), (void*)(first * ebo->GetIndexSize()));
}

VertexArray::~VertexArray()
{
	if (m_rendererID > 0) { 
		glDeleteVertexArrays(1, &m_rendererID);
		MM_INFO("Vertex array unloaded successfully; id=%u", m_rendererID);
	}
}

}
