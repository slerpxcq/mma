#include "CorePch.hpp"
#include "Renderer.hpp"

#include "Graphics/Shader.hpp"
#include "SubMesh.hpp"
#include "File/Image.hpp"
/* BEGIN TEST INCLUDE */
#include "File/Text.hpp"
#include "Viewport.hpp"
#include "Grid.hpp"
#include <glm/gtc/matrix_transform.hpp>
/* END TEST INCLUDE */

namespace mm
{

void Renderer::LoadInternalTextures()
{
	for (i32 i = 0; i <= 10; ++i) {
		std::stringstream ss{};
		const char* path = "../../resources/textures/";
		ss << path << "toon" << std::setw(2) << std::setfill('0') << i << ".bmp";
		auto img = Image(ss.str());
		auto tex = MakeRef<Texture2D>(img.GetWidth(), img.GetHeight(), 
									  Graphics::TexFormat::RGBA8);
		tex->SetSubImage(img.GetPixels(), 
						 Graphics::PixelType::UBYTE,
						 Graphics::PixelFormat::RGBA,
						 img.GetWidth(), img.GetHeight());
		m_internalTextures.push_back(tex);
	}
}

void Renderer::LoadDefaultProgram()
{
	auto vsSrc = Text("../../resources/shaders/test.vert");
	auto fsSrc = Text("../../resources/shaders/test.frag");
	auto vs = MakeRef<Shader>(vsSrc.GetString(), Graphics::ShaderType::VERTEX);
	auto fs = MakeRef<Shader>(fsSrc.GetString(), Graphics::ShaderType::FRAGMENT);
	m_defaultProgram = Ref<Program>(new Program{ vs, fs });
}

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
	auto program = m_defaultProgram;
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

