#pragma once

#include "../GPUResourceFactory.hpp"

namespace mm
{

class GPUResourceFactoryImpl_GL : public GPUResourceFactory::Impl
{
public:
	virtual Ref<Buffer> MakeBuffer() override;

private:
};

}

