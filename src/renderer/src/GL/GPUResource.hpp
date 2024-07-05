#pragma once

namespace mm
{

class GPUResource
{
public:
	uint32_t GetID() const { return m_rendererID; }

protected:
	GPUResource() {}
	GPUResource(const GPUResource&) = delete;

	GPUResource(GPUResource&& other) noexcept {
		m_rendererID = other.m_rendererID;
		other.m_rendererID = 0;
	}

	virtual	~GPUResource() {}

protected:
	uint32_t m_rendererID = 0;
};

}
