#pragma once

#include <glm/glm.hpp>

namespace mm
{

class VertexLayout
{
public:
	virtual ~VertexLayout() = default;
	virtual void Apply() const = 0;
	virtual u32 GetSize() const = 0;
};

}

