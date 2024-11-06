#include "CorePch.hpp"
#include "Texture.hpp"

#include "Graphics.hpp"

namespace mm
{
Texture::Texture(Target target) : 
	m_target{ target } 
{
	GetGraphics()->CreateTexture(*this);
}

Texture::~Texture() 
{ 
	GetGraphics()->DeleteTexture(*this); 
}

void Texture::Bind(u32 unit) const 
{
	GetGraphics()->BindTexture(*this, unit); 
}

}
