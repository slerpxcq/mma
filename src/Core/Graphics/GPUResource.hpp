#pragma once

namespace mm
{

class GPUResource
{
public:
	virtual ~GPUResource() = default;

	GPUResource() = default;
	GPUResource(const GPUResource&) = delete;
	GPUResource(GPUResource&&) = default;
	GPUResource& operator=(const GPUResource&) = delete;
	GPUResource& operator=(GPUResource&&) = default;

	u32 GetID() const { return m_id; }
	u32* GetIDPtr() { return &m_id; }

private:
	u32 m_id{};
};

}

