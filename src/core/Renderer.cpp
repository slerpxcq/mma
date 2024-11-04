#include "CorePch.hpp"
#include "Renderer.hpp"

#include "SubMesh.hpp"
/* BEGIN TEST INCLUDE */
#include "Viewport.hpp"
#include <glm/gtc/matrix_transform.hpp>
/* END TEST INCLUDE */

namespace mm
{

void Renderer::BeginFrame()
{
	auto& fb = GetMainViewport()->GetFrameBuffer();
	auto gfx = GetGraphics();
	fb.ClearColor(0);
	fb.ClearDepth();
	gfx->BindFrameBuffer(fb);
}

void Renderer::EndFrame()
{
	auto gfx = GetGraphics();
	gfx->UnbindFrameBuffer();
}

void Renderer::Render(SubMesh& subMesh)
{
	// TODO: Render submesh
	// TODO: Setup uniforms
	auto vp = GetMainViewport();
	auto& fb = vp->GetFrameBuffer();
	auto program = GetDefaultProgram();
	auto gfx = GetGraphics();
	program->SetUniform("u_viewProjection", vp->GetMatrix());
	program->Use();
	// TODO: Draw mesh
	gfx->DrawElements(subMesh.GetVertexArray(), subMesh.GetIndexBegin(), subMesh.GetIndexCount());
}

}

