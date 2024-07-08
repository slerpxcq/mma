#include "RendererPch.hpp"
#include "MMVertex.hpp"

namespace mm
{

void MMVertex::Startup()
{
	m_vertexLayout = std::make_unique<VertexLayout>();

	m_vertexLayout->Push({ "Position", GL_FLOAT, 3 });
	m_vertexLayout->Push({ "Normal", GL_FLOAT, 3 });

	m_vertexLayout->Push({ "texCoord", GL_FLOAT, 2 });
	m_vertexLayout->Push({ "texCoord1", GL_FLOAT, 2 });
	m_vertexLayout->Push({ "texCoord2", GL_FLOAT, 2 });
	m_vertexLayout->Push({ "texCoord3", GL_FLOAT, 2 });
	m_vertexLayout->Push({ "texCoord4", GL_FLOAT, 2 });

	m_vertexLayout->Push({ "Bones", GL_INT, 4 });
	m_vertexLayout->Push({ "Weights", GL_FLOAT, 3 });

	m_vertexLayout->Push({ "SDEF_C", GL_FLOAT, 3 });
	m_vertexLayout->Push({ "SDEF_R0", GL_FLOAT, 3 });
	m_vertexLayout->Push({ "SDEF_R1", GL_FLOAT, 3 });
}

}
