#pragma once

#include "GPUResource.hpp"

namespace mm
{

class Texture : public GPUResource
{
public:
	enum class Target {
		TEX_2D, TEX_CUBE
	};

public:
	virtual ~Texture() { GetGraphics()->DeleteTexture(*this); }
	Target GetTarget() const { return m_target; }
	void Bind(u32 unit) const { GetGraphics()->BindTexture(*this, unit); }

protected:
	Texture(Target target) : 
		m_target{ target } {
		GetGraphics()->CreateTexture(*this);
	}

protected:
	Target m_target{};
};

}

