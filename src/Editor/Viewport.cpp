#include "mmpch.hpp"
#include "Viewport.hpp"

#include "Core/GL/GLRenderer.hpp"

#include "EditorLayer.hpp"

namespace mm
{
	Viewport::Viewport(EditorLayer& editor) :
        m_editor(editor)
	{
        m_framebuffer = std::make_unique<GLFrameBuffer>(glm::uvec2(1, 1));
	}

    void Viewport::OnUpdate(float deltaTime)
    {
        m_camera.Update();
    }

	void Viewport::OnRender(GLRenderer& renderer)
	{
        renderer.SetCamera(m_camera);
        renderer.Viewport(m_size.x, m_size.y);
        renderer.BeginFramebuffer(m_framebuffer.get());
        renderer.Clear(glm::vec4(0.1, 0.1, 0.1, 1), GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        m_editor.GetWorld().Render(renderer);
        renderer.EndFramebuffer();
	}

	void Viewport::OnUIRender()
	{
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
        ImGui::Begin("Viewport", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBringToFrontOnFocus);

        ImVec2 min = ImGui::GetWindowContentRegionMin();
        ImVec2 max = ImGui::GetWindowContentRegionMax();
        glm::uvec2 size = glm::uvec2(max.x - min.x, max.y - min.y);
        glm::ivec2 pos = glm::ivec2(ImGui::GetWindowPos().x + min.x, ImGui::GetWindowPos().y + min.y);

        // resized
        if (m_size != size) {
            m_size = size;
            m_framebuffer->Reload(m_size);
            m_camera.SetAspect((float)m_size.x / m_size.y);
        }
        // moved
        if (m_pos != pos) {
            m_pos = pos;
        }

        ImGui::Image(
            (void*)m_framebuffer->GetColorTarget(),
            ImVec2(m_framebuffer->GetSize().x, m_framebuffer->GetSize().y),
            ImVec2(0, 1), ImVec2(1, 0));

        ImGui::End();
        ImGui::PopStyleVar();
	}
}
