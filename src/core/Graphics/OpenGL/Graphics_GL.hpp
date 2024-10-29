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
	virtual void SetBufferData(Buffer&, const void* data, u32 size) const override;
	virtual void SetBufferSubData(Buffer&, const void* data, u32 size, u32 offset) const override;
	virtual void SetBufferBindBase(Buffer&, u32 base) const override;

private:
};

}

