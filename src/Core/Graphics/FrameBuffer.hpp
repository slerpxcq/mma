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
	~FrameBuffer();

	u32 GetWidth() const { return m_width; }
	u32 GetHeight() const { return m_height; }
	f32 GetAspect() const { return static_cast<f32>(m_width) / m_height; }
	Vec2 GetSize() const { return { m_width, m_height }; }
	const Texture2D* GetAttachment(Graphics::AttachmentType type, u32 index) const; 
	void ClearColor(u32 index, Color color = { 0.f, 0.f, 0.f, 0.f }) const;
	void ClearDepth(f32 depth = 1.f, i32 stencil = 0) const;
	void Resize(u32 width, u32 height);
	bool IsComplete() const;

private:
	 void AddAttachment(Attachment); 

private:
	u32 m_width{};
	u32 m_height{};
	HashMap<i32, Pair<Attachment, Scoped<Texture2D>>> m_attachments{};
};

}

