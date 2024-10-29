#pragma once

#include "../Graphics.hpp"

namespace mm
{

class Graphics_GL : public Graphics
{
public:
	Graphics_GL() : Graphics{ Config::API::GL4 } {}

	virtual void CreateBuffer(Buffer&) const override;
	virtual void DeleteBuffer(Buffer&) const override;
	virtual void SetBufferData(const Buffer&, const void* data, u32 size) const override;
	virtual void SetBufferSubData(const Buffer&, const void* data, u32 size, u32 offset) const override;
	virtual void SetBufferBindBase(const Buffer&, u32 base) const override;
	virtual void CreateVertexArray(VertexArray&) const override;
	virtual void DeleteVertexArray(VertexArray&) const override;	
	virtual void SetIndexBuffer(const VertexArray&, const IndexBuffer&) const override;
	virtual void SetVertexBuffer(const VertexArray&, const VertexBuffer&) const override;
	virtual void SetVertexAttribFormat(const VertexArray&, u32 location, AttributeType type, 
									   u32 count, u32 offset, bool normalized = false) const override;

private:
};

}

