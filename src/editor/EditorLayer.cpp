#include "EditorPch.hpp"
#include "EditorLayer.hpp"

#include "Panel/ViewportPanel.hpp"

/* BEGIN TEST INCLUDE */
#include "Core/Graphics/Shader.hpp"
#include "Core/Graphics/Program.hpp"
#include "Core/File/Text.hpp"
#include "Core/ModelNode.hpp"
#include "Core/File/PMXFile.hpp"
/* END TEST INCLUDE */

#include <imgui.h>

namespace mm
{

EditorLayer::EditorLayer(const Window& window) :
	ImGuiLayer{ window }
{
	auto vp = MakeScoped<ViewportPanel>("Viewport");
	vp->SetFrameBuffer(GetMainFrameBuffer());
	m_panels.push_back(std::move(vp));

	/* BEGIN TEST CODE */
	// Load model
	auto root = GetRootNode();
	auto pmx =  PMXFile::Load("../../resources/model/つみ式ミクさん/000 ミクさん.pmx");
	auto model = ModelNode::LoadFromPMX(*pmx);
	// Load shader
	auto vsSrc = Text::Load("../../resources/shaders/test.vert");
	auto fsSrc = Text::Load("../../resources/shaders/test.frag");
	auto vs = MakeRef<Shader>(vsSrc->GetString(), Graphics::ShaderType::VERTEX);
	auto fs = MakeRef<Shader>(fsSrc->GetString(), Graphics::ShaderType::FRAGMENT);
	auto prog = Ref<Program>(new Program{ vs, fs });
	model->program = prog;
	root->AttachChild(std::move(model)); 
	/* END TEST CODE */
}

EditorLayer::~EditorLayer()
{
}

void EditorLayer::OnUpdate(f32 deltaTime)
{
	for (auto& panel : m_panels) {
		panel->OnUpdate(deltaTime);
	}
}

void EditorLayer::OnRender()
{
	Begin();

	for (auto& panel : m_panels) {
		panel->OnRender();
	}

	/* BEGIN TEST CODE */
	/* END TEST CODE */

	End();
}

}
