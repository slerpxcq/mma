#pragma once

#include "Core/MM/Math/Transform.hpp" 
#include "Bezier.hpp"

namespace mm
{
	class Model;
	class VMDFile;
	class PMXFile;

	class Animation
	{
	public:
		static constexpr uint32_t SUBFRAME_COUNT = 8;

		struct Keyframe {
		public:
			virtual ~Keyframe() {};
		protected:
			explicit Keyframe(uint32_t frame) :
				frame(frame) {}
		public:
			uint32_t frame;
		};

		struct BoneKeyframe : Keyframe {
			BoneKeyframe(uint32_t frame, const Transform& transform, const Bezier& bezier) :
				Keyframe(frame),
				transform(transform),
				bezier(bezier) {}
		public:
			Transform  transform;
			Bezier bezier;
		};

		struct MorphKeyframe : Keyframe {
			MorphKeyframe(uint32_t frame, float weight) :
				Keyframe(frame),
				weight(weight) {}
		public:
			float weight;
		};

	public:
		Animation(Model& model);
		void LoadFromFile(const std::filesystem::path& path);
		void Update(uint32_t frame, uint32_t subframe);
		const std::string& GetName() const { return m_name; }
		const auto& GetBoneKeyframeMatrix() const { return m_boneKeyframeMatrix; }
		const auto& GetMorphKeyframeMatrix() const { return m_morphKeyframeMatrix; }
		auto& GetBoneKeyframeMatrix() { return m_boneKeyframeMatrix; }
		auto& GetMorphKeyframeMatrix() { return m_morphKeyframeMatrix; }
		void InsertMorphKeyframe(uint32_t morphIndex, const MorphKeyframe& keyframe);
		void InsertBoneKeyframe(uint32_t boneIndex, const BoneKeyframe& keyframe);
		void RemoveMorphKeyframe(uint32_t morphIndex, uint32_t frame);
		void RemoveBoneKeyframe(uint32_t boneIndex, uint32_t frame);
		const Model& GetModel() const { return m_model; }

	private:
		void LoadBoneKeyframes();
		void LoadMorphKeyframes();
		void LoadDefaultBoneKeyframes();
		void LoadDefaultMorphKeyframes();
		void UpdateBoneTransforms(uint32_t frame, uint32_t subframe);
		void UpdateMorphWeights(uint32_t frame, uint32_t subframe);
		float Distance(uint32_t frame, uint32_t subframe, uint32_t prev, uint32_t next);

	private:
		Model& m_model;
		VMDFile* m_vmdFile;
		std::string m_name;

		std::vector<std::vector<BoneKeyframe>>  m_boneKeyframeMatrix;
		std::vector<std::vector<MorphKeyframe>> m_morphKeyframeMatrix;
	};

	 // For std::upper_bound and std::lower_bound
	static inline bool operator<(uint32_t lhs, const Animation::Keyframe& rhs) 
	{
		return lhs < rhs.frame;
	}

	static inline bool operator<(const Animation::Keyframe& lhs, uint32_t rhs) 
	{
		return lhs.frame < rhs;
	}

	// for std::sort
	static inline bool operator<(const Animation::Keyframe& lhs, const Animation::Keyframe& rhs)
	{
		return lhs.frame < rhs.frame;
	}
}

