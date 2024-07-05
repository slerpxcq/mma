#pragma once

#include "GPUResource.hpp"
#include "Texture2D.hpp"

namespace mm
{

class FrameBuffer : public GPUResource
{
	friend class Viewport;
public:
	static std::shared_ptr<FrameBuffer> CreateDefault(const std::string& name, glm::uvec2 size = { 1, 1 });
	FrameBuffer();
	FrameBuffer(FrameBuffer&& other) noexcept;
	~FrameBuffer();

	void Bind() const;
	void Resize(glm::uvec2 size);

	void Clear(glm::vec4 color, uint32_t bitmask) const;

	glm::uvec2 GetSize() const { return m_size; }

	std::shared_ptr<Texture2D> GetDepthStencilAttachment() { return m_depthStencilAttachment; }
	std::shared_ptr<Texture2D> GetColorAttachment(uint32_t index) { return m_colorAttachments[index]; }

private:
	glm::uvec2 m_size = glm::uvec2(1, 1);

	std::shared_ptr<Texture2D> m_depthStencilAttachment;
	std::vector<std::shared_ptr<Texture2D>> m_colorAttachments;
};

}


