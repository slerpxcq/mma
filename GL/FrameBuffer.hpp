#pragma once

#include "GPUResource.hpp"
#include "Handle.hpp"
#include "Texture2D.hpp"

#include <glm/glm.hpp>

namespace mm
{

class FrameBuffer : public GPUResource
{
public:
	FrameBuffer();
	FrameBuffer(FrameBuffer&& other) noexcept;
	~FrameBuffer();

	/* Default framebuffer with one color and one depth/stencil attachment */
	static FrameBuffer CreateDefault(glm::vec2 size);

	void Bind() const;
	void Resize(glm::uvec2 size);

	glm::uvec2 GetSize() const { return m_size; }

	Ref<Texture2D> GetDepthAttachment() { return m_depthAttachment; }
	Ref<Texture2D> GetColorAttachment(uint32_t index) { return m_colorAttachments[index]; }

private:
	glm::uvec2 m_size = glm::uvec2(1, 1);

	Ref<Texture2D> m_depthAttachment;
	std::vector<Ref<Texture2D>> m_colorAttachments;
};

}


