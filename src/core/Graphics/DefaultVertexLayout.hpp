#pragma once

#include "VertexLayout.hpp"

namespace mm
{

class DefaultVertexLayout : public VertexLayout
{
public:
	struct Struct {
		glm::vec3 position{};
		glm::vec3 normal{};
		glm::vec2 texCoord{};
		glm::vec2 texCoord1{};
		glm::vec2 texCoord2{};
		glm::vec2 texCoord3{};
		glm::vec2 texCoord4{};
		glm::ivec4 bones{};
		glm::vec3 weights{};
		glm::vec3 sdef_c{};
		glm::vec3 sdef_r0{};
		glm::vec3 sdef_r1{};
	};

public:
	static DefaultVertexLayout instance;
	void Apply() const override;
	u32 GetSize() const override { return sizeof(Struct); }
};

}

