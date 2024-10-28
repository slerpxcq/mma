#pragma once

namespace mm
{

class Buffer;

class Graphics
{
public:
	virtual	~Graphics() = default;
	Graphics(Config::API api);
	Config::API GetAPI() const { return m_api; }

	virtual void CreateBuffer(Buffer&) const = 0;
	virtual void DeleteBuffer(Buffer&) const = 0;
	virtual void SetBufferData(Buffer&, void* data, u32 size) const = 0;
	virtual void SetBufferSubData(Buffer&, void* data, u32 size, u32 offset) const = 0;
	virtual void SetBufferBindBase(Buffer&, u32 base) const = 0;

private:
	Config::API m_api{};
};

}

