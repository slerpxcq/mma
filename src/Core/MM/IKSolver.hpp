#pragma once

#include "Files/PMXFile.hpp"
#include "Transform.hpp"

namespace mm
{
	class Armature;
	class IKSolver
	{
		struct Node {
			int32_t index;
			Transform world;
			Transform local;
		};

	public:
		IKSolver(Armature& armature, const PMXFile::Bone::IK& ik, int32_t target);
		void Solve();
		void Sync();

	private:
		void Rotate(uint32_t index, const glm::quat& qL, const glm::quat& qW);

	private:
		Armature& m_armature;
		const PMXFile::Bone::IK& m_ik;

		std::vector<Node> m_nodes;
		int32_t           m_target;
	};

}

