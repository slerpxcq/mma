#pragma once

namespace mm
{
	class GLBuffer
	{
		friend class GLVertexArray;
	public:
		GLBuffer(uint32_t target);
		GLBuffer(const GLBuffer&) = delete;
		GLBuffer(GLBuffer&&) = default;
		~GLBuffer();

		void SetData(uint32_t size, void* data);
		void SetSubData(uint32_t offset, uint32_t size, void* data);
		void SetBase(uint32_t base);

		uint32_t GetId() const {
			return m_id;
		}

		uint32_t GetTarget() const {
			return m_target;
		}

	private:
		uint32_t m_size;
		uint32_t m_id;
		uint32_t m_target;
	};
}


