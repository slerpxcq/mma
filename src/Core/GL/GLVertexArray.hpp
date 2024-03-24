#pragma once

namespace mm
{
	class GLBuffer;
	class GLVertexAttrib;

	class GLVertexArray
	{
	public:
		GLVertexArray();
		~GLVertexArray();

		void BindBuffer(const GLBuffer& buffer);
		void Bind();
		void Attrib(GLVertexAttrib* attrib);

		void DrawArray(uint32_t mode, uint32_t first, uint32_t count);
		void DrawElem(uint32_t mode, uint32_t first, uint32_t count);

	private:
		uint32_t m_id;
	};
}

