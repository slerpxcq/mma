#pragma once

#include "GPUResource.hpp"

namespace mm
{

class Texture : public GPUResource
{
public:
	enum class Target {
		TEXTURE_2D, TEXTURE_CUBE, TEXTURE_2D_ARRAY
	};

public:
	virtual ~Texture();
	void Bind(u32 unit) const;

	Target GetTarget() const { return m_target; }

protected:
	Texture(Target target);

protected:
	Target m_target{};
};

}

