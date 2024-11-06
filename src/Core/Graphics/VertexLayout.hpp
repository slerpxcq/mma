#pragma once

#include <glm/glm.hpp>

namespace mm
{

class VertexArray;

class VertexLayout
{
public:
	virtual ~VertexLayout() = default;
	virtual void ApplyTo(const VertexArray&) const = 0;
	virtual u32 GetSize() const = 0;
};

}

