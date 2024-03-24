#pragma once

namespace mm
{
	class GLVertexAttrib
	{
		friend class GLVertexArray;
	protected:
		virtual void Set() const = 0;
	public:
		virtual ~GLVertexAttrib() {}
	};
}


