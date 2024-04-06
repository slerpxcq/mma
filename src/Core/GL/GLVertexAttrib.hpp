#pragma once

namespace mm
{
	class GLVertexArray;
	class GLVertexAttrib
	{
		friend class GLVertexArray;
	protected:
		virtual void Set(GLVertexArray& vao) const = 0;
	public:
		virtual ~GLVertexAttrib() {}
	};
}


