#pragma once

#include "GL/GLLayout.hpp"

namespace mm
{
	class SampleLayout : public GLLayout
	{
	public:
		struct Vertex {
			glm::vec2 pos;
			glm::vec4 color;
		};

	private:
		void Set() override {
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, pos));
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
		}
	};
}
