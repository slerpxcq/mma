#pragma once

#include <cstdint>
#include <vector>

namespace mm
{

class VertexLayout
{
	friend class VertexArray;
public:
	struct Attribute {
		char name[16];
		/* GL_FLOAT, GL_INT, ...*/
		uint32_t type;
		/* 1, 2, 3, 4 */
		uint32_t count;
	};

	void Push(const Attribute& attribute);

private:
	std::vector<Attribute> m_attributes;
};

}

