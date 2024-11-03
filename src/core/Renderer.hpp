#pragma once

namespace mm
{

class SubMesh;

class Renderer
{
public:
	class Visitor;

private:
};

class Renderer::Visitor
{
public:
	void Render(SubMesh&);
};

}

