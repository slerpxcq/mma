#include "EditorPch.hpp"
#include "PoseEditorOverlay.hpp"

#include "Core/SceneManager.hpp"
#include "Core/Model.hpp"
#include "Core/Armature.hpp"

namespace mm
{

void PoseEditorOverlay::OnUpdate(f32 deltaTime)
{
}

void PoseEditorOverlay::OnRender()
{
	/* BEGIN TEST CODE */
	auto sm = GetSceneManager();
	for (auto& obj : sm->GetObjects()) {
		if (auto model = dynamic_cast<const Model*>(obj.get()); model) {
			auto armature = model->GetArmature();
		}
	}
	/* END TEST CODE */
}

}

