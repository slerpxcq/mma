#include "EditorPch.hpp"
#include "ViewportPanel.hpp"

#include "Core/Viewport.hpp"
#include "Core/Node.hpp"
#include "Common/Math/Cast.hpp"

#include <imgui.h>

namespace mm
{

void ViewportPanel::OnUpdate(f32 deltaTime)
{
	if (m_viewport && m_resized) {
		m_viewport->GetFrameBuffer().Resize(m_contentSize.x, m_contentSize.y);
		MM_APP_TRACE("viewport resized; width={0}, height={1}", m_contentSize.x, m_contentSize.y);
	}
}

void ViewportPanel::OnRender()
{
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
	ImGui::Begin(m_name.c_str(), nullptr, 
				 ImGuiWindowFlags_NoCollapse | 
				 ImGuiWindowFlags_NoScrollbar |
				 ImGuiWindowFlags_NoScrollWithMouse);

	Panel::OnBegin();

	if (m_viewport) {
		ImGui::Image(reinterpret_cast<void*>(m_viewport->GetFrameBuffer().GetID()),
					 ImVec2(m_contentSize.x, m_contentSize.y),
					 ImVec2(0, 1), ImVec2(1, 0));
	} else {
		ImGui::Text("ERROR: no framebuffer is linked");
	}

	UpdateCamera();

	ImGui::End();
	ImGui::PopStyleVar();
}

void ViewportPanel::UpdateCamera() const
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

	auto& cam = m_viewport->GetCamera();
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
	if (std::fabs(io.MouseWheel) > std::numeric_limits<float>::epsilon()) {
		Vec3 translation = ZOOM_SPEED * io.MouseWheel * view;
		node->TranslateWorld(translation);
	}
}

}
