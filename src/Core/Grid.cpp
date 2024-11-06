#include "CorePch.hpp"
#include "Grid.hpp"

#include "Renderer.hpp"

namespace mm
{

void Grid::Render()
{
	GetRenderer()->Render(*this);
}

}
