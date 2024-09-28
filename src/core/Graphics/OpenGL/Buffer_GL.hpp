#pragma once

#include "../Buffer.hpp"

namespace mm
{

class Buffer_GL : public Buffer
{
public:
	Buffer_GL(Target target);
	~Buffer_GL();

	void SetData(void* data, u32 size) override;
	void SetSubData(void* data, u32 size, u32 offset) override;
	void SetBindBase(u32 base) override;

private:
};

}

