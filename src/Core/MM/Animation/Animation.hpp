#pragma once

#include "../Transform.hpp"
#include "../Files/VMDFile.hpp"
#include "../Files/PMXFile.hpp"
#include "Bezier.hpp"

namespace mm
{
	class Model;

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
			BoneKeyframe(uint32_t frame, const Transform& xform, const Bezier& bez) :
				Keyframe(frame),
				xform(xform),
				bez(bez) {}
		public:
			Transform  xform;
			Bezier bez;
		};

		struct FacialKeyframe : Keyframe {
			FacialKeyframe(uint32_t frame, float weight) :
				Keyframe(frame),
				weight(weight) {}
		public:
			float weight;
		};

	public:
		Animation(Model& model);
		Animation(Model &model, const std::filesystem::path& path);
		void Update(uint32_t frame, uint32_t subframe);
		const std::string& GetName() const { return m_name; }
		const auto& GetBoneKeyframes() const { return m_boneKeyframes; }
		const auto& GetMorphKeyframes() const { return m_morphKeyframes; }

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
		std::unique_ptr<VMDFile> m_vmdFile;
		std::string m_name;

		std::vector<std::vector<BoneKeyframe>>   m_boneKeyframes;
		std::vector<std::vector<FacialKeyframe>> m_morphKeyframes;
	};

	static inline bool operator==(uint32_t frame, const Animation::Keyframe& rhs)
	{
		return frame == rhs.frame;
	}

	static inline bool operator<(uint32_t frame, const Animation::Keyframe& rhs)
	{
		return frame < rhs.frame;
	}

	static inline bool operator<(const Animation::Keyframe& rhs, uint32_t lhs)
	{
		return rhs.frame < lhs;
	}

	static inline bool operator<(const Animation::Keyframe& lhs, const Animation::Keyframe& rhs)
	{
		return lhs.frame < rhs.frame;
	}
}

