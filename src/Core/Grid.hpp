#pragma once

#include "SceneObject.hpp"
#include "Renderable.hpp"

namespace mm
{

class Grid : public Renderable
{
public:
	/* BEGIN TEST CODE */
	Grid(Ref<Material> mat) : 
		Renderable{ mat } {}
	/* END TEST CODE */
	void Render() override;

private:
};

}

