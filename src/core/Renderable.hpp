#pragma once

#include "Material.hpp"
#include "Renderer.hpp"

namespace mm
{

class Renderable
{
public:
	virtual ~Renderable() = default;
	Renderable(Ref<Material> mat) :
		m_material{ mat } {}

	virtual void Render(/*Renderer&*/) = 0;
	const Material& GetMaterial() const { return *m_material; }

private:
	Ref<Material> m_material{};
};

}

