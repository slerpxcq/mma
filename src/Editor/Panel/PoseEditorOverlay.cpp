#include "EditorPch.hpp"
#include "PoseEditorOverlay.hpp"

#include "Core/SceneManager.hpp"
#include "Core/Model.hpp"
#include "Core/Armature.hpp"

#include "ViewportPanel.hpp"
#include "Core/Viewport.hpp"

#include "Common/Math/Cast.hpp"

#include <imgui.h>
#include <ImGuizmo.h>

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

static void DrawRect(ImDrawList* drawList, 
					 Vec2 screenPos, f32 radius, 
					 u32 fillColor, u32 outlineColor, 
					 u32 outlineSize)
{
	ImVec2 p1 = ImVec2(screenPos.x - radius, screenPos.y - radius);
	ImVec2 p2 = ImVec2(screenPos.x + radius, screenPos.y + radius);
	drawList->AddRectFilled(p1, p2, fillColor);
	drawList->AddRect(p1, p2, outlineColor, 0, 0, outlineSize);
}

static void DrawTriangle(ImDrawList* drawList,
						 Vec2 screenPos, f32 radius,
						 u32 fillColor, u32 outlineColor, 
						 u32 outlineSize)
{
	static constexpr float HALF_SQRT3 = 0.866f;
	ImVec2 p1 = ImVec2(screenPos.x - HALF_SQRT3 * radius, screenPos.y + 0.5f * radius);
	ImVec2 p2 = ImVec2(screenPos.x, screenPos.y - radius);
	ImVec2 p3 = ImVec2(screenPos.x + HALF_SQRT3 * radius, screenPos.y + 0.5f * radius);
	drawList->AddTriangleFilled(p1, p2, p3, fillColor);
	drawList->AddTriangle(p1, p2, p3, outlineColor, outlineSize);
}

static void DrawCircle(ImDrawList* drawList,
					   Vec2 screenPos, f32 radius,
					   u32 fillColor, u32 outlineColor,
					   u32 outlineSize)
{
	drawList->AddCircleFilled(Cast<ImVec2>(screenPos), radius, fillColor);
	drawList->AddCircle(Cast<ImVec2>(screenPos), radius, outlineColor, 0, outlineSize);
}

static void DrawBoneConnection(ImDrawList* drawList,
							   Vec2 begin, Vec2 end, f32 radius,
							   u32 outlineColor, u32 outlineSize)
{
	Vec2 d = glm::normalize(begin - end);
	Vec2 n = { -d.y, d.x };
	Vec2 p1 = begin + radius * n;
	Vec2 p2 = begin - radius * n;
	drawList->AddLine(Cast<ImVec2>(p2), Cast<ImVec2>(end), outlineColor, outlineSize);
	drawList->AddLine(Cast<ImVec2>(end), Cast<ImVec2>(p1), outlineColor, outlineSize);
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
				//if (bone->GetFlags() & Bone::Flags::VISIBLE_BIT) {
				if (true) {
					Vec2 screenPos = ToScreenPos(bone->GetNode()->GetWorldTranslation());
					if (bone->GetFlags() & Bone::Flags::MOVEABLE_BIT) {
						DrawRect(drawList, screenPos, 
								 BUTTON_RADIUS, FILL_COLOR, OUTLINE_COLOR, OUTLINE_SIZE);
					} 
					else if (bone->GetFlags() & Bone::Flags::FIXED_AXIS_BIT) {
						DrawTriangle(drawList, screenPos, 
									 BUTTON_RADIUS, FILL_COLOR, OUTLINE_COLOR, OUTLINE_SIZE);
					}
					else if (bone->GetFlags() & Bone::Flags::ROTATABLE_BIT) {
						DrawCircle(drawList, screenPos, 
								   BUTTON_RADIUS, FILL_COLOR, OUTLINE_COLOR, OUTLINE_SIZE);
					}
					DrawBoneConnection(drawList, screenPos, GetTipPos(bone), 
									   BUTTON_RADIUS, OUTLINE_COLOR, OUTLINE_SIZE);
				}
			}
		}
	}
	/* END TEST CODE */
}

Vec3 PoseEditorOverlay::ToScreenPos(Vec3 worldPos)
{
	auto& panel = static_cast<ViewportPanel&>(m_parent);
	Viewport* viewport = panel.GetViewport();
	Mat4 viewProjection = viewport->GetViewProjectionMatrix();
	Vec2 viewportSize = panel.GetContentSize();
	Vec2 viewportPos = panel.GetContentPos();
	Vec4 ndcPos = viewProjection * Vec4(worldPos, 1);
	ndcPos /= ndcPos.w;
	return Vec3((ndcPos.x + 1.f) * 0.5f * viewportSize.x + viewportPos.x,
				(-ndcPos.y + 1.f) * 0.5f * viewportSize.y + viewportPos.y,
				ndcPos.z);
}

Vec2 PoseEditorOverlay::GetTipPos(Bone* bone)
{
	if (bone->GetFlags() & Bone::Flags::CONNECTED_BIT) {
		auto end = bone->GetTipInfoBone();
		if (end) {
			return ToScreenPos(end->GetNode()->GetWorldTranslation());
		} else {
			return ToScreenPos(bone->GetNode()->GetWorldTranslation());
		}
	}
	else {
		return ToScreenPos(bone->GetNode()->GetWorldTranslation() +
							 glm::rotate(bone->GetNode()->GetWorldRotation(),
										 bone->GetTipInfoOffset()));
	}
}

void PoseEditorOverlay::Edit()
{
	auto& panel = static_cast<ViewportPanel&>(m_parent);
	auto viewport = panel.GetViewport();
	Mat4 view = viewport->GetViewMatrix();
	Mat4 proj = viewport->GetProjectionMatrix();
}

}

