#pragma once

namespace mm
{

class Buffer;
class VertexArray;
class IndexBuffer;
class VertexBuffer;

class Graphics
{
public:
	enum class AttributeType { FLOAT, INT };
public:
	virtual	~Graphics() = default;
	Graphics(Config::API api);
	Config::API GetAPI() const { return m_api; }

	virtual void CreateBuffer(Buffer&) const = 0;
	virtual void DeleteBuffer(Buffer&) const = 0;
	virtual void SetBufferData(const Buffer&, const void* data, u32 size) const = 0;
	virtual void SetBufferSubData(const Buffer&, const void* data, u32 size, u32 offset) const = 0;
	virtual void SetBufferBindBase(const Buffer&, u32 base) const = 0;
	virtual void CreateVertexArray(VertexArray&) const = 0;
	virtual void DeleteVertexArray(VertexArray&) const = 0;
	virtual void SetIndexBuffer(const VertexArray&, const IndexBuffer&) const = 0;
	virtual void SetVertexBuffer(const VertexArray&, const VertexBuffer&) const = 0;
	virtual void SetVertexAttribFormat(const VertexArray&, u32 location, AttributeType type, 
									   u32 count, u32 offset, bool normalized = false) const = 0;

private:
	Config::API m_api{};
};

}

