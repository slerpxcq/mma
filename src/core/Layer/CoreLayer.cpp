#include "CorePch.hpp"
#include "CoreLayer.hpp"

#include "../Graphics/Graphics.hpp"

/* BEGIN TEST INCLUDE */
#include "../Node.hpp"
#include "../Model.hpp"
#include "../Graphics/Program.hpp"
#include "../Graphics/FrameBuffer.hpp"
#include "SceneManager.hpp"

#include <glm/gtc/matrix_transform.hpp>
/* END TEST INCLUDE */

namespace mm
{

void CoreLayer::OnUpdate(f32 deltaTime)
{
}

void CoreLayer::OnRender()
{
	/* BEGIN TEST CODE */
	auto fb = GetMainFrameBuffer();
	auto gfx = GetGraphics();
	fb->ClearColor(0);
	fb->ClearDepth();
	gfx->BindFrameBuffer(*fb);
	auto model = dynamic_cast<Model*>(GetSceneManager()->GetRootNode().GetChildren().back().get());
	if (model) {
		// // TODO: Setup uniforms
		// auto program = model->program;
		// program->Use();
		// f32 aspect = static_cast<f32>(fb->GetWidth()) / fb->GetHeight();
		// Mat4 view = glm::lookAt(Vec3{ 0, 10, -30 }, Vec3{ 0, 10, 0 }, Vec3{ 0, 1, 0 });
		// Mat4 proj = glm::perspective(glm::radians(60.f), aspect, .1f, 100.f);
		// Mat4 vp = proj * view;
		// program->SetUniform("u_viewProjection", vp);
		// // TODO: Draw each mesh 
		// for (const auto& mesh : model->GetMeshes()) {
		// 	const auto& va = mesh->GetVertexArray();
		// 	gfx->DrawElements(va, mesh->GetIndexOffset(), mesh->GetIndexCount());
		// }
	}
	gfx->UnbindFrameBuffer();
	/* END TEST CODE */
}

}
