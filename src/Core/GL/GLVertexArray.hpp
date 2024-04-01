#pragma once

namespace mm
{
	class GLBuffer;
	class GLVertexAttrib;

	class GLVertexArray
	{
	public:
		GLVertexArray();
		GLVertexArray(const GLVertexArray&) = delete;
		GLVertexArray(GLVertexArray&&) = default;
		~GLVertexArray();

		void Bind() const ;
		void Unbind() const;

		void SetVertexBuffer(const GLBuffer& vbo, uint32_t vertexSize);

		void SetElemBuffer(const GLBuffer& ebo);
		void SetVertexAttrib(const GLVertexAttrib& attrib);
		void SetElemType(uint32_t type);

		void DrawArray(uint32_t mode, uint32_t first, uint32_t count) const;
		void DrawElem(uint32_t mode, uint32_t first, uint32_t count) const;

		uint32_t GetId() const {
			return m_id;
		}

	private:
		uint32_t m_id;
		uint32_t m_elemSize;
		uint32_t m_elemType;
	};
}

