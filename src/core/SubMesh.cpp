#include "CorePch.hpp"
#include "SubMesh.hpp"

#include "Renderer.hpp"

namespace mm
{

void SubMesh::Render()
{
	auto renderer = GetRenderer();
	renderer->Render(*this);
}

}

