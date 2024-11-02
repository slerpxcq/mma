#pragma once

#include "Texture2D.hpp"

namespace mm
{

class FrameBuffer : public GPUResource
{
public:
	struct Attachment {
		Graphics::AttachmentType type;
		u32 index;
		Graphics::TexFormat format;
	};

public:
	FrameBuffer(u32 width, u32 height, 
				InitList<Attachment> attachments);
	~FrameBuffer() { GetGraphics()->DeleteFrameBuffer(*this); }

	const Texture2D* GetAttachment(Graphics::AttachmentType type, u32 index) const;
	void ClearColor(u32 index, Color color = { 0.f, 0.f, 0.f, 0.f }) const { 
		GetGraphics()->ClearFrameBufferColor(*this, index, color); 
	}
	void ClearDepth(f32 depth = 0.f, i32 stencil = 0) const { 
		GetGraphics()->ClearFrameBufferDepth(*this, depth, stencil); 
	}
	void Resize(u32 width, u32 height);
	void Bind() const { GetGraphics()->BindFrameBuffer(*this); }
	bool IsComplete() const { 
		return (GetGraphics()->CheckFrameBufferStatus(*this) 
				== Graphics::FrameBufferStatus::OK); 
	}

private:
	 void AddAttachment(Graphics::AttachmentType type, u32 index, Graphics::TexFormat format);

private:
	u32 m_width{};
	u32 m_height{};
	HashMap<i32, Scoped<Texture2D>> m_attachments{};
};

}

