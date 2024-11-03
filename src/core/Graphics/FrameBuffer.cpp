#include "CorePch.hpp"
#include "FrameBuffer.hpp"

namespace mm
{

FrameBuffer::FrameBuffer(u32 width, u32 height, 
						 InitList<Attachment> attachments) :
	m_width{ width }, m_height{ height } 
{
	auto gfx = GetGraphics();
	gfx->CreateFrameBuffer(*this);
	for (const auto& attachment : attachments) {
		AddAttachment(attachment);
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

void FrameBuffer::AddAttachment(Attachment attachment) 
{
	auto gfx = GetGraphics();
	auto tex = MakeScoped<Texture2D>(m_width, m_height, attachment.format);
	gfx->FrameBufferTexture(*this, *tex, attachment.type, attachment.index);
	m_attachments.insert({ ToKey(attachment.type, attachment.index), 
						 std::make_pair(attachment, std::move(tex)) });
}

const Texture2D* FrameBuffer::GetAttachment(Graphics::AttachmentType attachment, u32 index) const
{
	auto it = m_attachments.find(ToKey(attachment, index));
	if (it == m_attachments.end()) {
		return nullptr;
	} else {
		return it->second.second.get();
	}
}

void FrameBuffer::Resize(u32 width, u32 height)
{
	auto gfx = GetGraphics();
	for (auto&& [_, pair] : m_attachments) {
		auto&& [attachment, tex] = pair;
		tex->Resize(width, height);
		gfx->FrameBufferTexture(*this, *tex, attachment.type, attachment.index);
	}
	m_width = width;
	m_height = height;
}

}
