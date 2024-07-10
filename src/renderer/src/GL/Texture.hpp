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

private:
	uint32_t m_target;
};

}

