#include "CorePch.hpp"
#include "MeshNode.hpp"

#include <renderer/src/GPUResourceManager.hpp> 
#include <renderer/src/GL/Shader.hpp> 

namespace mm
{

void MeshNode::Render()
{
	auto shader = GPUResourceManager::Instance().Get<Shader>("Default shader");
	MM_ASSERT(shader);
	shader->Use();
	m_vertexArray->DrawElements(GL_TRIANGLES, m_indexOffset, m_indexCount);
}

}

