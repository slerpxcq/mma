#pragma once

namespace mm
{
	class GLLayout
	{
		friend class GLVertexArray;
	protected:
		virtual void Set() = 0;
	public:
		virtual ~GLLayout() {}
	};
}


