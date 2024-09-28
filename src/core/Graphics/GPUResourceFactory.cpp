#include "CorePch.hpp"
#include "GPUResourceFactory.hpp"

#include "OpenGL/GPUResourceFactoryImpl_GL.hpp"

namespace mm
{

Scoped<GPUResourceFactory::Impl> GPUResourceFactory::MakeImpl()
{
	switch (GetGraphicsAPI()) {
	case Config::GraphicsAPI::GL4:
		return MakeScoped<GPUResourceFactoryImpl_GL>();
	default:
		break;
	}
}

}

