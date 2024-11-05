#include "CorePch.hpp"
#include "Renderer.hpp"

#include "SubMesh.hpp"
/* BEGIN TEST INCLUDE */
#include "Viewport.hpp"
#include "Grid.hpp"
#include <glm/gtc/matrix_transform.hpp>
/* END TEST INCLUDE */

namespace mm
{

void Renderer::BeginFrame()
{
	auto gfx = GetGraphics();
	auto& fb = GetMainViewport()->GetFrameBuffer();
	fb.ClearColor(0);
	fb.ClearDepth();
	gfx->SetViewport(fb.GetSize());
	gfx->BindFrameBuffer(fb);
	gfx->SetDepthTest(true);
	gfx->SetBlend(true);
	gfx->SetBlendFunc(Graphics::BlendFactor::SRC_ALPHA,
					  Graphics::BlendFactor::ONE_MINUS_SRC_ALPHA);
	gfx->SetFrontFace(Graphics::FrontFace::CCW);
}

void Renderer::EndFrame()
{
	auto gfx = GetGraphics();
	gfx->UnbindFrameBuffer();
	gfx->SetDepthTest(false);
	gfx->SetBlend(false);
	gfx->SetCulling(false);
}

void Renderer::Render(SubMesh& subMesh)
{
	// Setup uniforms
	auto vp = GetMainViewport();
	auto& fb = vp->GetFrameBuffer();
	auto program = GetDefaultProgram();
	auto gfx = GetGraphics();
	program->SetUniform("u_viewProjection", vp->GetMatrix());
	program->SetUniform("u_albedo", 0);
	program->Use();

	// Bind textures
	auto& mat = subMesh.GetMaterial();
	auto& albedo = mat.GetMap(Material::MapType::ALBEDO);
	albedo.Bind(0);

	gfx->SetCulling(!(mat.GetFlags() & Material::NO_CULL_BIT));

	// Draw mesh
	gfx->DrawElements(subMesh.GetVertexArray(), subMesh.GetIndexBegin(), subMesh.GetIndexCount());
}

void Renderer::Render(Grid& grid)
{
	auto gfx = GetGraphics();
	auto& program = grid.GetMaterial().GetProgram();
	auto vp = GetMainViewport();
	program.SetUniform("u_viewProjection", vp->GetMatrix());
	program.Use();

	gfx->DrawArrays(Graphics::DrawMode::LINES, 0, 204 + 6);
}

}

