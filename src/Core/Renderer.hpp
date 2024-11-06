#pragma once

namespace mm
{

class SubMesh;
class Grid;

class Renderer
{
public:
	void BeginFrame();
	void EndFrame();
	void Render(SubMesh&);
	void Render(Grid&);

private:
};

}

