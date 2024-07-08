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
	VertexArray(std::shared_ptr<VertexBuffer> vbo, 
				std::shared_ptr<ElementBuffer> ebo,
				const VertexLayout& layout);

	~VertexArray();

	void DrawArrays(uint32_t mode, uint32_t first, uint32_t count) const;
	void DrawElements(uint32_t mode, uint32_t first, uint32_t count) const;

private:
	/* returns stride */
	uint32_t SetLayout(const VertexLayout& layout);

private:
	std::shared_ptr<ElementBuffer> m_elementBuffer;
	std::shared_ptr<VertexBuffer>  m_vertexBuffer;
};

}

