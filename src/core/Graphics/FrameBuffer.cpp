#include "CorePch.hpp"
#include "FrameBuffer.hpp"

namespace mm
{

FrameBuffer::FrameBuffer(u32 width, u32 height) :
	m_width{ width }, m_height{ height }
{
	const auto gfx = GetGraphics();
	gfx->CreateFrameBuffer(*this);
}

static i32 ToKey(Graphics::Attachment attachment, u32 index)
{
	switch (attachment) {
	case Graphics::Attachment::DEPTH:
		return -1;
		break;
	case Graphics::Attachment::COLOR:
		return index;
		break;
	default:
		MM_CORE_UNINPLEMENTED();
	}
}

void FrameBuffer::AddAttachment(Graphics::Attachment attachment, u32 index, Graphics::TexFormat format) 
{
	const auto gfx = GetGraphics();
	auto tex = MakeScoped<Texture2D>(m_width, m_height, format);
	gfx->FrameBufferTexture(*this, *tex, attachment, index);
	m_attachments.insert({ ToKey(attachment, index), std::move(tex) });
}

const Texture2D* FrameBuffer::GetAttachment(Graphics::Attachment attachment, u32 index) const
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
