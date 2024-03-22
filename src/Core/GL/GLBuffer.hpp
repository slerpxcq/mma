#pragma once

namespace mm
{
	class GLBuffer
	{
		friend class GLVertexArray;
	public:
		GLBuffer(uint32_t target);
		virtual ~GLBuffer();

		void Data(uint32_t size, void* data);
		void SubData(uint32_t offset, uint32_t size, void* data);
		void Base(uint32_t base);

	private:
		uint32_t m_id;
		uint32_t m_target;
	};
}


