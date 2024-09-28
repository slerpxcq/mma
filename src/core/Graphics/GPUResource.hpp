#pragma once

namespace mm
{

class GPUResource
{
public:
	u32 GetID() { return m_id; }

protected:
	u32 m_id;
};

}

