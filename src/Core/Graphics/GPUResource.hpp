#pragma once

namespace mm
{

class GPUResource
{
public:
	MM_NONCOPYABLE(GPUResource)

	virtual ~GPUResource() = default;
	GPUResource() = default;

	u32 GetID() const { return m_id; }
	u32* GetIDPtr() { return &m_id; }

private:
	u32 m_id{};
};

}

