#pragma once

#include "SceneObject.hpp"
#include "Renderable.hpp"

namespace mm
{

class Grid : public Renderable
{
public:
	Grid(Ref<Material> mat) : 
		Renderable{ mat } {}
	void Render() override;

private:
};

}

