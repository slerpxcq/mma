#include "mmpch.hpp"
#include "Animation.hpp"
#include "Core/Locale/Locale.hpp"
#include "../Model/Model.hpp"

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
		m_boneKeyframes.resize(m_model.GetPMXFile().GetBones().size());

		for (uint32_t i = 0; i < vmdMotionDatas.size(); ++i) {
			const auto& data = vmdMotionDatas[i];
			std::string boneName = Locale::ShiftJISToUTF8(data.boneName, sizeof(data.boneName));

			const auto& dict = m_model.GetArmature().GetDict();
			auto it = dict.find(boneName);

			if (it != dict.end()) {
				int32_t boneIndex = it->second;
				m_boneKeyframes[boneIndex].emplace_back(
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

		for (uint32_t i = 0; i < m_boneKeyframes.size(); ++i) {
			if (m_boneKeyframes[i].empty()) {
				MM_INFO("{0}: bone has no motion data", m_model.GetPMXFile().GetBoneName(i));
				m_boneKeyframes[i].push_back({ 0, Transform::identity(), Bezier() });
			}

			std::sort(m_boneKeyframes[i].begin(), m_boneKeyframes[i].end());
		}
	}

	void Animation::LoadMorphKeyframes()
	{
		const auto& vmdMorphDatas = m_vmdFile->GetMorphDatas();
		m_morphKeyframes.resize(m_model.GetPMXFile().GetMorphs().size());

		for (uint32_t i = 0; i < vmdMorphDatas.size(); ++i) {
			const auto& data = vmdMorphDatas[i];
			std::string morphName = Locale::ShiftJISToUTF8(data.morphName, sizeof(data.morphName));

			const auto& dict = m_model.GetMorph().GetDict();
			auto it = dict.find(morphName);

			if (it != dict.end()) {
				int32_t morphIndex = it->second;
				m_morphKeyframes[morphIndex].emplace_back(data.frameNumber, data.weight);
			}
			else {
				MM_INFO("{0}: {1}: morph does not exist in model", m_name, morphName);
				continue;
			}
		}

		for (uint32_t i = 0; i < m_morphKeyframes.size(); ++i) {
			if (m_morphKeyframes[i].empty()) {
				MM_INFO("{0}: morph has no motion data", m_model.GetPMXFile().GetMorphName(i));
				m_morphKeyframes[i].push_back({ 0, 0.0f });
			}

			std::sort(m_morphKeyframes[i].begin(), m_morphKeyframes[i].end());
		}
	}

	Animation::Animation(Model& model, const std::filesystem::path& path) :
		m_vmdFile(std::make_unique<VMDFile>(path)),
		m_model(model)
	{
		m_name = path.filename().u8string();

		LoadBoneKeyframes();
		LoadMorphKeyframes();

		MM_INFO("{0}: animation loaded", m_name);
	}

	void Animation::Update(uint32_t frame, uint32_t subframe)
	{
		UpdateBoneTransforms(frame, subframe);
		UpdateMorphWeights(frame, subframe);
	}

	void Animation::UpdateMorphWeights(uint32_t frame, uint32_t subframe)
	{
		for (uint32_t i = 0; i < m_morphKeyframes.size(); ++i) {
			const auto& kfs = m_morphKeyframes[i];
			auto prev = FindPrev(kfs, frame);
			float& weight = m_model.GetMorph().GetWeights()[i];

			if (prev == kfs.cend()) {
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
		for (uint32_t i = 0; i < m_boneKeyframes.size(); ++i) {
			auto& kfs = m_boneKeyframes[i];
			auto prev = FindPrev(kfs, frame);
			Transform& xform = m_model.GetArmature().GetPose()[i];

			if (prev == kfs.cend()) {
				std::advance(prev, -1);
				xform = prev->xform;
			}
			else {
				auto next = prev;
				std::advance(prev, -1);
				float dist = Distance(frame, subframe, prev->frame, next->frame);
				glm::vec4 weights = prev->bez.Eval(dist);
				xform.trans = glm::mix(prev->xform.trans, next->xform.trans, glm::vec3(weights));
				xform.rot = glm::slerp(prev->xform.rot, next->xform.rot, weights.w);
			}
		}
	}
}
