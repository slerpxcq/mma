#pragma once

#include "Texture2D.hpp"

namespace mm
{

class FrameBuffer : public GPUResource
{
public:
	FrameBuffer(u32 width, u32 height);
	~FrameBuffer() { GetGraphics()->DeleteFrameBuffer(*this); }

	void AddAttachment(Graphics::Attachment attachment, u32 index, Graphics::TexFormat format);
	const Texture2D* GetAttachment(Graphics::Attachment attachment, u32 index) const;
	void ClearColor(u32 index, Color color = { 0.f, 0.f, 0.f, 0.f }) const { GetGraphics()->ClearFrameBufferColor(*this, index, color); }
	void ClearDepth(f32 depth = 0.f, i32 stencil = 0) const { GetGraphics()->ClearFrameBufferDepth(*this, depth, stencil); }
	void Resize(u32 width, u32 height);
	bool IsComplete() const { return (GetGraphics()->CheckFrameBufferStatus(*this) == Graphics::FrameBufferStatus::OK); }

private:
	u32 m_width{};
	u32 m_height{};
	HashMap<i32, Scoped<Texture2D>> m_attachments{};
};

}

