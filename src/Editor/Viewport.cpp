#include "mmpch.hpp"
#include "Viewport.hpp"

#include "Core/MM/Renderer/Renderer.hpp"

#include "EditorLayer.hpp"
#include "Core/ResourceManager/ResourceManager.hpp"

#include "Core/Utility/Type.hpp"

namespace mm
{
	Viewport::Viewport(EditorLayer& editor) :
        m_editor(editor),
        m_cameraController(*this, m_editor.GetWorld().GetCamera())
	{
        //m_framebuffer = std::make_unique<GLFrameBuffer>(glm::uvec2(1, 1));
        m_framebufferMS = std::make_unique<GLFrameBufferMS>(glm::uvec2(1, 1));
        m_grid = std::make_unique<Grid>(*this);
	}

    void Viewport::OnUpdate(float deltaTime)
    {
        m_cameraController.Update();
    }

	void Viewport::OnRender(Renderer& renderer)
	{
        renderer.SetCamera(m_cameraController.GetCamera());
        //renderer.SetFramebuffer(m_framebuffer.get());
        //renderer.SetFramebuffer(m_framebufferMS.get());
        m_framebufferMS->Bind();
        //glViewport(0, 0, m_framebuffer->GetSize().x, m_framebuffer->GetSize().y);
        glViewport(0, 0, m_framebufferMS->GetSize().x, m_framebufferMS->GetSize().y);
        glClearColor(.05, .05, .05, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Skybox; SHOULD BE BELONG TO WORLD
        //glDepthMask(GL_FALSE);
        //renderer.SetShader(ResourceManager::Instance().GetShader("skybox"));
        //renderer.GetActiveShader()->Uniform("u_skybox", 0);
        //ResourceManager::Instance().GetTexture("skybox")->Bind(0);
        //glDrawArrays(GL_TRIANGLES, 0, 36);
        //glDepthMask(GL_TRUE);

        m_grid->Render(renderer);
        m_editor.GetWorld().Render(renderer);
        renderer.SetFramebuffer(nullptr);
	}

	void Viewport::OnUIRender()
	{
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
        ImGui::Begin("Viewport", nullptr, 
            ImGuiWindowFlags_NoCollapse | 
            ImGuiWindowFlags_NoBringToFrontOnFocus |
            ImGuiWindowFlags_NoScrollbar |
            ImGuiWindowFlags_NoScrollWithMouse);

        m_hovered = ImGui::IsWindowHovered();

        ImVec2 min = ImGui::GetWindowContentRegionMin();
        ImVec2 max = ImGui::GetWindowContentRegionMax();
        ImVec2 size = max - min;
        ImVec2 pos = ImGui::GetWindowPos() + min;

        if (m_size != size) {
            m_size = size;
            //m_framebuffer->Reload(glm::uvec2(size.x, size.y));
            m_framebufferMS->Reload(glm::uvec2(size.x, size.y));
            m_cameraController.GetCamera().SetAspect((float)size.x / size.y);
        }

        m_framebufferMS->Blit();
        ImGui::Image(
            (void*)m_framebufferMS->GetColorTarget(),
            ImVec2(m_framebufferMS->GetSize().x, m_framebufferMS->GetSize().y),
            ImVec2(0, 1), ImVec2(1, 0));

        //ImGui::Image(
        //    (void*)m_framebuffer->GetColorTarget(),
        //    ImVec2(m_framebuffer->GetSize().x, m_framebuffer->GetSize().y),
        //    ImVec2(0, 1), ImVec2(1, 0));

        ImGui::End();
        ImGui::PopStyleVar();
	}
}
