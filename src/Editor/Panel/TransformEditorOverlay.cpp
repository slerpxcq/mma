#include "EditorPch.hpp"
#include "TransformEditorOverlay.hpp"

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

TransformEditorOverlay::TransformEditorOverlay(Panel& parent, StringView name) :
	PanelOverlay{ parent, name }
{
	MM_APP_ASSERT(dynamic_cast<ViewportPanel*>(&parent));
}

void TransformEditorOverlay::OnUpdate(f32 deltaTime)
{
}

static ImGuizmo::OPERATION ToImGuizmoOperation(TransformEditorOverlay::GizmoContext::Operation op)
{
	switch (op) {
	case TransformEditorOverlay::GizmoContext::Operation::ROTATE:	     return ImGuizmo::ROTATE;
	case TransformEditorOverlay::GizmoContext::Operation::ROTATE_X:      return ImGuizmo::ROTATE_X;
	case TransformEditorOverlay::GizmoContext::Operation::ROTATE_Y:      return ImGuizmo::ROTATE_Y;
	case TransformEditorOverlay::GizmoContext::Operation::ROTATE_Z:      return ImGuizmo::ROTATE_Z;
	case TransformEditorOverlay::GizmoContext::Operation::ROTATE_SCREEN: return ImGuizmo::ROTATE_SCREEN;
	case TransformEditorOverlay::GizmoContext::Operation::TRANSLATE:     return ImGuizmo::TRANSLATE;
	case TransformEditorOverlay::GizmoContext::Operation::TRANSLATE_X:   return ImGuizmo::TRANSLATE_X;
	case TransformEditorOverlay::GizmoContext::Operation::TRANSLATE_Y:   return ImGuizmo::TRANSLATE_Y;
	case TransformEditorOverlay::GizmoContext::Operation::TRANSLATE_Z:   return ImGuizmo::TRANSLATE_Z;
	default: MM_APP_UNREACHABLE(); 
	}
}

static ImGuizmo::MODE ToImGuizmoMode(TransformEditorOverlay::GizmoContext::Mode mode)
{
	switch (mode) {
	case TransformEditorOverlay::GizmoContext::Mode::LOCAL: return ImGuizmo::LOCAL;
	case TransformEditorOverlay::GizmoContext::Mode::WORLD: return ImGuizmo::WORLD;
	default: MM_APP_UNREACHABLE();
	}
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

static bool ShowButton(Vec2 screenPos, f32 radius, i32 index)
{
	char buf[32];
	std::snprintf(buf, sizeof(buf), "Bone_%u", index);
	ImVec2 cursorPos = ImVec2(screenPos.x - radius,
							  screenPos.y - radius);
	ImGui::SetCursorScreenPos(cursorPos);
	ImGui::SetItemAllowOverlap();
	//return ImGui::InvisibleButton(buf, ImVec2(2 * radius, 2 * radius));
	return ImGui::Button(buf, ImVec2(2 * radius, 2 * radius));
}

void TransformEditorOverlay::OnRender()
{
	/* BEGIN TEST CODE */
	auto sm = GetSceneManager();
	auto drawList = ImGui::GetWindowDrawList();
	for (auto& obj : sm->GetObjects()) {
		if (auto model = dynamic_cast<const Model*>(obj.get()); model) {
			auto armature = model->GetArmature();
			i32 index{};
			for (auto bone : armature->GetBones()) {
				if (bone->GetFlags() & Bone::Flags::VISIBLE_BIT) {
					Vec2 screenPos = ToScreenPos(bone->GetNode()->GetWorldTranslation());
					if (ShowButton(screenPos, BUTTON_RADIUS, index++)) {
						OnBoneSelected(bone);
					}
					if (bone->GetFlags() & Bone::Flags::MOVEABLE_BIT) {
						DrawRect(drawList, screenPos, 
								 BUTTON_RADIUS, FILL_COLOR, OUTLINE_COLOR, OUTLINE_SIZE);
					} 
					else if (bone->GetFixedAxis()) {
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
	ShowGizmo();
	/* END TEST CODE */
}

Vec3 TransformEditorOverlay::ToScreenPos(Vec3 worldPos)
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

Vec2 TransformEditorOverlay::GetTipPos(Bone* bone)
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

void TransformEditorOverlay::ShowGizmo()
{
	auto& panel = static_cast<ViewportPanel&>(m_parent);
	auto viewport = panel.GetViewport();
	Mat4 view = viewport->GetViewMatrix();
	Mat4 proj = viewport->GetProjectionMatrix();

	ImGuizmo::SetDrawlist();
	Vec2 pos = panel.GetContentPos();
	Vec2 size = panel.GetContentSize();
	ImGuizmo::SetRect(pos.x, pos.y, size.x, size.y);

	/* temp */
	m_gizmoContext.operation = GizmoContext::Operation::ROTATE;

	if (m_gizmoContext.bone) {
		ImGuizmo::Enable(true);
		switch (m_gizmoContext.constraint) {
		case GizmoContext::Constraint::FIXED:
			ImGuizmo::Manipulate(glm::value_ptr(view),
								 glm::value_ptr(proj),
								 ImGuizmo::ROTATE_X,
								 ImGuizmo::LOCAL,
								 glm::value_ptr(m_gizmoContext.localFrame));
			break;
		case GizmoContext::Constraint::LOCAL:
			ImGuizmo::Manipulate(glm::value_ptr(view),
								 glm::value_ptr(proj),
								 ToImGuizmoOperation(m_gizmoContext.operation),
								 ImGuizmo::LOCAL,
								 glm::value_ptr(m_gizmoContext.localFrame));
			break;
		case GizmoContext::Constraint::FREE:
			ImGuizmo::Manipulate(glm::value_ptr(view),
								 glm::value_ptr(proj),
								 ToImGuizmoOperation(m_gizmoContext.operation),
								 ToImGuizmoMode(m_gizmoContext.mode),
								 glm::value_ptr(m_gizmoContext.localFrame));
			break;
		default:
			MM_APP_UNREACHABLE();
		}
		Mat4 world = m_gizmoContext.localFrame * m_gizmoContext.localFrameInverse;
		Mat4 pose = m_gizmoContext.worldToLocal * world;
		m_gizmoContext.bone->SetPoseLocal(pose);
	} else {
		ImGuizmo::Enable(false);
	}
}

void TransformEditorOverlay::OnBoneSelected(Bone* bone)
{
	m_gizmoContext.bone = bone;
	m_gizmoContext.worldToLocal = (bone->GetParentWorld() * bone->GetBindLocal()).Inverse();

	auto fixedAxis = bone->GetFixedAxis();
	auto localAxes = bone->GetLocalAxes();
	m_gizmoContext.constraint = 
		localAxes.has_value() ? GizmoContext::Constraint::LOCAL :
		fixedAxis.has_value() ? GizmoContext::Constraint::FIXED :
		GizmoContext::Constraint::FREE;

	Mat4 localFrame{ 1.f };
	switch (m_gizmoContext.constraint) {
	case GizmoContext::Constraint::FIXED:
		Vec3 x = *fixedAxis;
		Vec3 z = glm::normalize(glm::cross(Vec3{ 0, 1, 0 }, x));
		Vec3 y = glm::cross(z, x);
		localFrame[0] = Vec4{ x, 0 };
		localFrame[1] = Vec4{ y, 0 };
		localFrame[2] = Vec4{ z, 0 };
		break;
	case GizmoContext::Constraint::LOCAL:
		localFrame = *localAxes;
		break;
	}
	m_gizmoContext.localFrame = bone->GetNode()->GetWorldMatrix() * localFrame;
	m_gizmoContext.localFrameInverse = glm::inverse(localFrame);
}

}

