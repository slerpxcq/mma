#pragma once

namespace mm
{

class Buffer;

class GPUResourceFactory
{
	MM_DECL_SINGLETON(GPUResourceFactory)
public:
	class Impl {
	public:
		~Impl() = default;

		virtual Ref<Buffer> MakeBuffer() = 0;
	private:
	};

	static Scoped<GPUResourceFactory::Impl> MakeImpl();

private:
	Scoped<Impl> m_impl{ MakeImpl() };
};


}

