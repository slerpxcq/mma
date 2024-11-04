#pragma once

namespace mm
{

class SubMesh;

class Renderer
{
public:
	void BeginFrame();
	void EndFrame();
	void Render(SubMesh&);

private:
};

}

