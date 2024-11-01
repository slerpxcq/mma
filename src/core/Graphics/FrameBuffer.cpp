#include "CorePch.hpp"
#include "FrameBuffer.hpp"

namespace mm
{

FrameBuffer::FrameBuffer(u32 width, u32 height, 
						 InitList<Attachment> attachments) :
	m_width{ width }, m_height{ height } 
{
	const auto gfx = GetGraphics();
	gfx->CreateFrameBuffer(*this);
	for (const auto& attachment : attachments) {
		AddAttachment(attachment.type, attachment.index, attachment.format);
	}
}

static i32 ToKey(Graphics::AttachmentType type, u32 index)
{
	switch (type) {
	case Graphics::AttachmentType::DEPTH:
		return -1;
		break;
	case Graphics::AttachmentType::COLOR:
		return index;
		break;
	default:
		MM_CORE_UNINPLEMENTED();
	}
}

void FrameBuffer::AddAttachment(Graphics::AttachmentType type, u32 index, Graphics::TexFormat format) 
{
	const auto gfx = GetGraphics();
	auto tex = MakeScoped<Texture2D>(m_width, m_height, format);
	gfx->FrameBufferTexture(*this, *tex, type, index);
	m_attachments.insert({ ToKey(type, index), std::move(tex) });
}

const Texture2D* FrameBuffer::GetAttachment(Graphics::AttachmentType attachment, u32 index) const
{
	auto it = m_attachments.find(ToKey(attachment, index));
	if (it == m_attachments.end()) {
		return nullptr;
	} else {
		return it->second.get();
	}
}

void FrameBuffer::Resize(u32 width, u32 height)
{
	MM_CORE_UNINPLEMENTED();
}

}
