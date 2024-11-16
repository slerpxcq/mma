#pragma once

namespace mm
{

class Collider 
{
public:
	union ConstructInfo {
		enum { BOX, SPHERE, CAPSULE } type;
		struct { f32 width, height, depth; } box;
		struct { f32 radius; } sphere;
		struct { f32 radius, height; } capsule;
	};

public:
	void SetHandle(void* handle) { m_handle = handle; }
	void* GetHandle() const { return m_handle; }

protected:
	void* m_handle{};
};

}

