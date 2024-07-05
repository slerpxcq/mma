#pragma once

#include "GPUResource.hpp"

namespace mm
{

class Texture : public GPUResource
{
public:
	Texture(uint32_t target);
	Texture(Texture&&) noexcept;
	virtual ~Texture();

	void Bind(uint32_t unit) const;
	uint64_t GetHandle() const { 
		MM_ASSERT(m_handle && "Invalid texture handle!");
		return m_handle; 
	}

protected:
	void CreateHandle();

private:
	uint32_t m_target;
	uint64_t m_handle = 0;
};

}

