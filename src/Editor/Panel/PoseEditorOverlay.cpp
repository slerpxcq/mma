#include "EditorPch.hpp"
#include "PoseEditorOverlay.hpp"

#include "Core/SceneManager.hpp"
#include "Core/Model.hpp"
#include "Core/Armature.hpp"

#include "ViewportPanel.hpp"
#include "Core/Viewport.hpp"

#include "Common/Math/Cast.hpp"

#include <imgui.h>

namespace mm
{

PoseEditorOverlay::PoseEditorOverlay(Panel& parent, StringView name) :
	PanelOverlay{ parent, name }
{
	MM_APP_ASSERT(dynamic_cast<ViewportPanel*>(&parent));
}

void PoseEditorOverlay::OnUpdate(f32 deltaTime)
{
}

void PoseEditorOverlay::OnRender()
{
	/* BEGIN TEST CODE */
	auto sm = GetSceneManager();
	auto drawList = ImGui::GetWindowDrawList();
	for (auto& obj : sm->GetObjects()) {
		if (auto model = dynamic_cast<const Model*>(obj.get()); model) {
			auto armature = model->GetArmature();
			for (auto bone : armature->GetBones()) {
				if (bone->GetFlags() & Bone::Flags::VISIBLE_BIT) {
					Vec2 pos = ToScreenPos(bone->GetNode()->GetWorldTranslation());
					if (bone->GetFlags() & Bone::Flags::MOVEABLE_BIT) {
						drawList->AddRectFilled(
							ImVec2(pos.x - BUTTON_RADIUS, pos.y - BUTTON_RADIUS),
							ImVec2(pos.x + BUTTON_RADIUS, pos.y + BUTTON_RADIUS), 
							FILL_COLOR);
						drawList->AddRect(
							ImVec2(pos.x - BUTTON_RADIUS, pos.y - BUTTON_RADIUS),
							ImVec2(pos.x + BUTTON_RADIUS, pos.y + BUTTON_RADIUS), 
							OUTLINE_COLOR, 0, 0, OUTLINE_SIZE);
					} 
					else if (bone->GetFlags() & Bone::Flags::FIXED_AXIS_BIT) {
					}
					else if (bone->GetFlags() & Bone::Flags::ROTATABLE_BIT) {
						drawList->AddCircleFilled(Cast<ImVec2>(pos), BUTTON_RADIUS, FILL_COLOR);
						drawList->AddCircle(Cast<ImVec2>(pos), BUTTON_RADIUS, OUTLINE_COLOR, 0, OUTLINE_SIZE);
					}

					// Vec2 tipPos;
					// if (bone->GetFlags() & Bone::Flags::CONNECTED_BIT) {
					// 	tipPos = ToScreenPos(bone->GetTipInfo().bone->GetNode()->GetWorldTranslation());
					// }
					// else {
					// }
				}
			}
		}
	}
	/* END TEST CODE */
}

/* z is reserved for overlap selection */
Vec3 PoseEditorOverlay::ToScreenPos(Vec3 pos)
{
	auto& panel = static_cast<ViewportPanel&>(m_parent);
	Viewport* viewport = panel.GetViewport();
	Mat4 viewProjection = viewport->GetMatrix();
	Vec2 viewportSize = panel.GetContentSize();
	Vec2 viewportPos = panel.GetContentPos();

	Vec4 ndcPos = viewProjection * Vec4(pos, 1);
	ndcPos /= ndcPos.w;
	return Vec3((ndcPos.x + 1.f) * 0.5f * viewportSize.x + viewportPos.x,
				(-ndcPos.y + 1.f) * 0.5f * viewportSize.y + viewportPos.y,
				ndcPos.z);
}

}

