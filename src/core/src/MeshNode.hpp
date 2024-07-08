#pragma once

#include "Node.hpp"

#include <renderer/src/GL/ElementBuffer.hpp>
#include <renderer/src/GL/VertexBuffer.hpp>
#include <renderer/src/GL/VertexArray.hpp>

namespace mm
{

class MeshNode : public Node
{
	friend class ModelLoader;
public:
	MeshNode(const std::string& name = "", 
			   std::shared_ptr<Node> parent = nullptr) :
		Node(name, parent) {}

private:
	uint32_t m_indexOffset;
	std::shared_ptr<ElementBuffer> m_elementBuffer;
	std::shared_ptr<VertexBuffer> m_vertexBuffer;
	std::shared_ptr<VertexArray> m_vertexArray;
};

}

