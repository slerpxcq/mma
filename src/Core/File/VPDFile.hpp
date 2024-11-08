#pragma once

#include "File.hpp"
#include "Common/Math/Transform.hpp"

namespace mm
{

class VPDFile : public File
{
public:
	explicit VPDFile(const Path& path);

	const auto& GetPose() const { return m_pose; }
	Transform GetTransform(StringView boneName) const;

private:
	HashMap<String, Transform> m_pose;
};

}

