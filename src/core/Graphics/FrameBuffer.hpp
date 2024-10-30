#pragma once

#include "Texture2D.hpp"

namespace mm
{

class FrameBuffer : public GPUResource
{
public:
	FrameBuffer(u32 width, u32 height);
	void AddAttachment(Graphics::Attachment attachment, u32 index, Graphics::TexFormat format);
	const Texture2D* GetAttachment(Graphics::Attachment attachment, u32 index) const;
	bool IsComplete() const { return (GetGraphics()->CheckFrameBufferStatus(*this) == Graphics::FrameBufferStatus::OK); }
	~FrameBuffer() { GetGraphics()->DeleteFrameBuffer(*this); }

private:
	u32 m_width{};
	u32 m_height{};
	HashMap<i32, Scoped<Texture2D>> m_attachments{};
};

}

