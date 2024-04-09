#include "mmpch.hpp"
#include "Viewport.hpp"

#include "Core/GL/GLRenderer.hpp"

#include "EditorLayer.hpp"
#include "Core/ResourceManager/ResourceManager.hpp"

namespace mm
{
	Viewport::Viewport(EditorLayer& editor) :
        m_editor(editor),
        m_cameraController(*this, m_editor.GetWorld().GetCamera())
	{
        m_framebuffer = std::make_unique<GLFrameBuffer>(glm::uvec2(1, 1));
        m_grid = std::make_unique<Grid>(*this);
	}

    void Viewport::OnUpdate(float deltaTime)
    {
        m_cameraController.Update();
    }

	void Viewport::OnRender(GLRenderer& renderer)
	{
        renderer.SetCamera(m_cameraController.GetCamera());
        renderer.SetFramebuffer(m_framebuffer.get());
        glViewport(0, 0, m_framebuffer->GetSize().x, m_framebuffer->GetSize().y);
        glClearColor(.05, .05, .05, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Skybox
        glDepthMask(GL_FALSE);
        renderer.SetShader(ResourceManager::s_instance.GetShader("skybox"));
        renderer.GetShader()->Uniform("u_skybox", 0);
        ResourceManager::s_instance.GetTexture("skybox")->Bind(0);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glDepthMask(GL_TRUE);

        m_grid->Render(renderer);
        m_editor.GetWorld().Render(renderer);
        renderer.SetFramebuffer(nullptr);
	}

	void Viewport::OnUIRender()
	{
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
        ImGui::Begin("Viewport", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBringToFrontOnFocus);

        m_hovered = ImGui::IsWindowHovered();

        ImVec2 min = ImGui::GetWindowContentRegionMin();
        ImVec2 max = ImGui::GetWindowContentRegionMax();
        glm::uvec2 size = glm::uvec2(max.x - min.x, max.y - min.y);
        glm::ivec2 pos = glm::ivec2(ImGui::GetWindowPos().x + min.x, ImGui::GetWindowPos().y + min.y);

        if (m_size != size) {
            m_size = size;
            m_framebuffer->Reload(m_size);
            m_cameraController.GetCamera().SetAspect((float)m_size.x / m_size.y);
        }
        if (m_pos != pos) {
            m_pos = pos;
        }

        ImGui::Image(
            (void*)m_framebuffer->GetColorTarget(),
            ImVec2(m_framebuffer->GetSize().x, m_framebuffer->GetSize().y),
            ImVec2(0, 1), ImVec2(1, 0));

        ImGui::End();
        ImGui::PopStyleVar();

        m_cameraController.OnUIRender();
	}
}
