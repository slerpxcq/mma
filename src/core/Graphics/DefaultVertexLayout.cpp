#include "CorePch.hpp"
#include "DefaultVertexLayout.hpp"

namespace mm
{

DefaultVertexLayout DefaultVertexLayout::instance;

void DefaultVertexLayout::ApplyTo(const VertexArray& va) const
{
	const auto& gfx = GetGraphics();

	gfx->SetVertexAttribFormat(va, 0, Graphics::AttributeType::FLOAT, 3, offsetof(Struct, position));
	gfx->SetVertexAttribFormat(va, 1, Graphics::AttributeType::FLOAT, 3, offsetof(Struct, normal));
	gfx->SetVertexAttribFormat(va, 2, Graphics::AttributeType::FLOAT, 2, offsetof(Struct, texCoord));
	gfx->SetVertexAttribFormat(va, 3, Graphics::AttributeType::FLOAT, 2, offsetof(Struct, texCoord1));
	gfx->SetVertexAttribFormat(va, 4, Graphics::AttributeType::FLOAT, 2, offsetof(Struct, texCoord2));
	gfx->SetVertexAttribFormat(va, 5, Graphics::AttributeType::FLOAT, 2, offsetof(Struct, texCoord3));
	gfx->SetVertexAttribFormat(va, 6, Graphics::AttributeType::FLOAT, 2, offsetof(Struct, texCoord4));
	gfx->SetVertexAttribFormat(va, 7, Graphics::AttributeType::INT, 4, offsetof(Struct, bones));
	gfx->SetVertexAttribFormat(va, 8, Graphics::AttributeType::FLOAT, 3, offsetof(Struct, weights));
	gfx->SetVertexAttribFormat(va, 9, Graphics::AttributeType::FLOAT, 3, offsetof(Struct, sdef_c));
	gfx->SetVertexAttribFormat(va, 10, Graphics::AttributeType::FLOAT, 3, offsetof(Struct, sdef_r0));
	gfx->SetVertexAttribFormat(va, 11, Graphics::AttributeType::FLOAT, 3, offsetof(Struct, sdef_r1));
}

}
