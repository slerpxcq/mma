#include "EditorPch.hpp"
#include "CameraControllerOverlay.hpp"

#include "ViewportPanel.hpp"
#include "Core/Viewport.hpp"
#include "Core/Node.hpp"

#include "Common/Math/Cast.hpp"

#include <imgui.h>

namespace mm
{

CameraControllerOverlay::CameraControllerOverlay(Panel& parent, StringView name) :
	PanelOverlay{ parent, name }
{
	MM_APP_ASSERT(dynamic_cast<ViewportPanel*>(&parent));
}

void CameraControllerOverlay::OnUpdate(f32 deltaTime)
{
}

void CameraControllerOverlay::OnRender()
{
	static constexpr float PAN_SPEED = 0.01f;
	static constexpr float ORBIT_SPEED = 0.005f;
	static constexpr float ZOOM_SPEED = 1.f;
	constexpr Vec3 CAM_PIVOT = { 0, 0, 0 };

	static Vec3 translationOnClick;
	static Vec3 viewOnClick;
	static Vec3 upOnClick;
	static Vec3 rightOnClick;
	static Quat rotationOnClick;

	static Vec2 mousePosOnClick;

	auto& panel = static_cast<ViewportPanel&>(m_parent);
	auto& cam = panel.GetViewport()->GetCamera();
	auto camNode = cam.GetNode();
	Vec3 view = cam.GetViewVector();
	Vec3 right = cam.GetRightVector();
	Vec3 up = cam.GetUpVector();
	Vec2 mousePos = Cast<Vec2>(ImGui::GetMousePos());
	auto node = cam.GetNode();

	if (ImGui::IsMouseClicked(ImGuiMouseButton_Right) || 
		ImGui::IsMouseClicked(ImGuiMouseButton_Middle)) {
		mousePosOnClick = Cast<Vec2>(ImGui::GetMousePos());
		translationOnClick = camNode->GetWorldTransform().translation;
		rotationOnClick = camNode->GetWorldTransform().rotation;
		upOnClick = cam.GetUpVector();
		rightOnClick = cam.GetRightVector();
	}

	if (ImGui::IsMouseDown(ImGuiMouseButton_Middle)) {
		Vec2 delta = mousePos - mousePosOnClick;
		Vec3 translation = PAN_SPEED * (delta.y * up + -delta.x * right);
		node->SetWorldTranslation(translationOnClick + translation);
	}

	if (ImGui::IsMouseDown(ImGuiMouseButton_Right)) {
		Vec2 delta = mousePos - mousePosOnClick;
		delta *= ORBIT_SPEED;
		Quat qY = glm::angleAxis(-delta.x, Vec3{ 0, 1, 0 });
		Quat qX = glm::angleAxis(-delta.y, rightOnClick);
		Quat q = qY * qX;
		Vec3 pivotToCameraOnClick = translationOnClick - CAM_PIVOT;
		node->SetWorldTransform({ CAM_PIVOT + q * pivotToCameraOnClick,  q * rotationOnClick });
	}

	auto& io = ImGui::GetIO();
	if (io.MouseWheel != 0.f) {
		Vec3 translation = ZOOM_SPEED * io.MouseWheel * view;
		node->TransformWorld(translation);
	}
}

}
