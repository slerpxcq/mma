#include "mmpch.hpp"
#include "Animation.hpp"
#include "Core/Locale/Locale.hpp"
#include "../Model/Model.hpp"

#include "Core/ResourceManager/ResourceManager.hpp"

namespace mm
{
	float Animation::Distance(uint32_t frame, uint32_t subframe, uint32_t prev, uint32_t next)
	{
		return (frame + ((float)subframe / SUBFRAME_COUNT) - prev) / (next - prev);
	}

	template <typename T>
	decltype(auto) FindPrev(const std::vector<T>& vec, uint32_t frame)
	{
		return std::upper_bound(
			vec.begin(),
			vec.end(),
			frame);
	}

	void Animation::LoadBoneKeyframes()
	{
		const auto& vmdMotionDatas = m_vmdFile->GetMotionDatas();
		m_boneKeyframeMatrix.clear();
		m_boneKeyframeMatrix.resize(m_model.GetPMXFile().GetBones().size());

		for (uint32_t i = 0; i < vmdMotionDatas.size(); ++i) {
			const auto& data = vmdMotionDatas[i];
			std::string boneName = Locale::ShiftJISToUTF8(data.boneName, sizeof(data.boneName));

			const auto& dict = m_model.GetArmature().GetDict();
			auto it = dict.find(boneName);

			if (it != dict.end()) {
				int32_t boneIndex = it->second;
				m_boneKeyframeMatrix[boneIndex].emplace_back(
					data.frameNumber,
					Transform(glm::make_vec3(data.position),
						glm::make_quat(data.rotation)),
					Bezier(data.interpolation));
			}
			else {
				MM_INFO("{0}: {1}: bone does not exist in model", m_name, boneName);
				continue;
			}
		}

		for (uint32_t i = 0; i < m_boneKeyframeMatrix.size(); ++i) {
			if (m_boneKeyframeMatrix[i].empty()) {
				MM_INFO("{0}: bone has no motion data", m_model.GetPMXFile().GetBoneName(i));
				m_boneKeyframeMatrix[i].emplace_back(0, Transform::identity(), Bezier());
			}

			std::sort(m_boneKeyframeMatrix[i].begin(), m_boneKeyframeMatrix[i].end());
		}
	}

	void Animation::LoadMorphKeyframes()
	{
		const auto& vmdMorphDatas = m_vmdFile->GetMorphDatas();
		m_morphKeyframeMatrix.clear();
		m_morphKeyframeMatrix.resize(m_model.GetPMXFile().GetMorphs().size());

		for (uint32_t i = 0; i < vmdMorphDatas.size(); ++i) {
			const auto& data = vmdMorphDatas[i];
			std::string morphName = Locale::ShiftJISToUTF8(data.morphName, sizeof(data.morphName));

			const auto& dict = m_model.GetMorph().GetDict();
			auto it = dict.find(morphName);

			if (it != dict.end()) {
				int32_t morphIndex = it->second;
				m_morphKeyframeMatrix[morphIndex].emplace_back(data.frameNumber, data.weight);
			}
			else {
				MM_INFO("{0}: {1}: morph does not exist in model", m_name, morphName);
				continue;
			}
		}

		for (uint32_t i = 0; i < m_morphKeyframeMatrix.size(); ++i) {
			if (m_morphKeyframeMatrix[i].empty()) {
				MM_INFO("{0}: morph has no motion data", m_model.GetPMXFile().GetMorphName(i));
				m_morphKeyframeMatrix[i].emplace_back(0, 0.0f);
			}

			std::sort(m_morphKeyframeMatrix[i].begin(), m_morphKeyframeMatrix[i].end());
		}
	}

	Animation::Animation(Model& model) :
		m_model(model)
	{
		m_name = "default";

		LoadDefaultBoneKeyframes();
		LoadDefaultMorphKeyframes();

		MM_INFO("{0}: default animation loaded", m_model.GetPMXFile().GetInfo().nameJP);
	}

	void Animation::LoadDefaultBoneKeyframes() 
	{
		m_boneKeyframeMatrix.clear();
		m_boneKeyframeMatrix.resize(m_model.GetPMXFile().GetBones().size());

		for (uint32_t i = 0; i < m_boneKeyframeMatrix.size(); ++i) {
			m_boneKeyframeMatrix[i].emplace_back(0, Transform::identity(), Bezier());
		}
	}

	void Animation::LoadDefaultMorphKeyframes()
	{
		m_morphKeyframeMatrix.clear();
		m_morphKeyframeMatrix.resize(m_model.GetPMXFile().GetMorphs().size());

		for (uint32_t i = 0; i < m_morphKeyframeMatrix.size(); ++i) {
			m_morphKeyframeMatrix[i].emplace_back(0, 0.0f);
		}
	}

	void Animation::InsertMorphKeyframe(uint32_t morphIndex, const MorphKeyframe& keyframe)
	{
		auto& keyframes = m_morphKeyframeMatrix[morphIndex];
		auto it = FindPrev(keyframes, keyframe.frame);
		keyframes.insert(it, keyframe);
	}

	void Animation::InsertBoneKeyframe(uint32_t boneIndex, const BoneKeyframe& keyframe)
	{
		auto& keyframes = m_boneKeyframeMatrix[boneIndex];
		auto it = FindPrev(keyframes, keyframe.frame);
		keyframes.insert(it, keyframe);
	}

	void Animation::LoadFromFile(const std::filesystem::path& path)
	{
		m_name = path.filename().u8string();

		auto vmd = std::make_unique<VMDFile>(path);
		m_vmdFile = vmd.get();
		ResourceManager::Instance().LoadFile(std::move(vmd));

		LoadBoneKeyframes();
		LoadMorphKeyframes();

		MM_INFO("{0}: animation loaded", m_model.GetPMXFile().GetInfo().nameJP);
	}

	void Animation::Update(uint32_t frame, uint32_t subframe)
	{
		UpdateBoneTransforms(frame, subframe);
		UpdateMorphWeights(frame, subframe);
	}

	void Animation::UpdateMorphWeights(uint32_t frame, uint32_t subframe)
	{
		for (uint32_t i = 0; i < m_morphKeyframeMatrix.size(); ++i) {
			const auto& keyframes = m_morphKeyframeMatrix[i];
			auto prev = FindPrev(keyframes, frame);
			float& weight = m_model.GetMorph().GetWeights()[i];

			if (prev == keyframes.cend()) {
				std::advance(prev, -1);
				weight = prev->weight;
			}
			else {
				auto next = prev;
				std::advance(prev, -1);
				float dist = Distance(frame, subframe, prev->frame, next->frame);
				weight = glm::mix(prev->weight, next->weight, dist);
			}
		}
	}

	void Animation::UpdateBoneTransforms(uint32_t frame, uint32_t subframe)
	{
		for (uint32_t i = 0; i < m_boneKeyframeMatrix.size(); ++i) {
			auto& keyframes = m_boneKeyframeMatrix[i];
			auto prev = FindPrev(keyframes, frame);
			Transform& transform = m_model.GetArmature().GetPose()[i];

			if (prev == keyframes.cend()) {
				std::advance(prev, -1);
				transform = prev->transform;
			}
			else {
				auto next = prev;
				std::advance(prev, -1);
				float dist = Distance(frame, subframe, prev->frame, next->frame);
				glm::vec4 weights = prev->bezier.Eval(dist);
				transform.translation = glm::mix(prev->transform.translation, next->transform.translation, glm::vec3(weights));
				transform.rotation = glm::slerp(prev->transform.rotation, next->transform.rotation, weights.w);
			}
		}
	}
}
