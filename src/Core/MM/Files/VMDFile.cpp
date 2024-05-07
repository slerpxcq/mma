#include "mmpch.hpp"
#include "VMDFile.hpp"

#include "Core/Utility/Stream.hpp"

#include "Core/MM/Animation/Animation.hpp"
#include "Core/MM/Model/Model.hpp"

namespace mm
{
	void VMDFile::ParseHeader(std::ifstream& stream)
	{
		Read(stream, m_header.magic);
		if (memcmp(m_header.magic, MAGIC, sizeof(MAGIC)))
			throw VMDParseError("Unknown magic");
		Read(stream, m_header.modelName);
	}

	void VMDFile::ParseMotionData(std::ifstream& stream)
	{
		uint32_t motionDataCount;
		Read(stream, motionDataCount);
		m_motionDatas.resize(motionDataCount);

		for (uint32_t i = 0; i < motionDataCount; ++i) {
			MotionData& motionData = m_motionDatas[i];
			Read(stream, motionData.boneName);
			Read(stream, motionData.frameNumber);
			Read(stream, motionData.position);
			Read(stream, motionData.rotation);
			Read(stream, motionData.interpolation);
		}
	}

	void VMDFile::ParseMorphData(std::ifstream& stream)
	{
		uint32_t morphDataCount;
		Read(stream, morphDataCount);
		m_morphDatas.resize(morphDataCount);

		for (uint32_t i = 0; i < morphDataCount; ++i) {
			MorphData& morphData = m_morphDatas[i];
			Read(stream, morphData.morphName);
			Read(stream, morphData.frameNumber);
			Read(stream, morphData.weight);
		}
	}

	VMDFile::VMDFile(const std::filesystem::path& path) 
	{
		m_path = path;
		std::ifstream stream;
		stream.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		stream.open(path, std::ifstream::binary);
		ParseHeader(stream);
		ParseMotionData(stream);
		ParseMorphData(stream);
	}

	VMDFile::VMDFile(const Animation& anim)
	{
		LoadHeader(anim);
		LoadMotionData(anim);
		LoadMorphData(anim);
		MM_INFO("VMD loaded from animation");
	}

	/* Convert name to ShiftJIS */
	static void LoadName(const std::string& u8string, uint32_t bufSize, char* buf)
	{
		std::string sjfstr = Locale::UTF8ToShiftJIS(u8string);
		std::memcpy(buf, sjfstr.c_str(), std::min(sjfstr.size(), (size_t)bufSize));
	}

	void VMDFile::LoadHeader(const Animation& anim)
	{
		std::memcpy(m_header.magic, MAGIC, sizeof(MAGIC));
		std::string modelName = anim.GetModel().GetPMXFile().GetName();
		/* Name should be in ShiftJIS */
		LoadName(modelName, sizeof(m_header.modelName), m_header.modelName);
	}
	
	static void LoadBezier(const Bezier& bezier, uint8_t* interpolation)
	{
		/* See https://w.atwiki.jp/kumiho_k/pages/15.html */
		uint8_t x_ax = bezier.GetHandles()[0][0].x;
		uint8_t x_ay = bezier.GetHandles()[0][0].y;
		uint8_t x_bx = bezier.GetHandles()[0][1].x;
		uint8_t x_by = bezier.GetHandles()[0][1].y;

		uint8_t y_ax = bezier.GetHandles()[1][0].x;
		uint8_t y_ay = bezier.GetHandles()[1][0].y;
		uint8_t y_bx = bezier.GetHandles()[1][1].x;
		uint8_t y_by = bezier.GetHandles()[1][1].y;

		uint8_t z_ax = bezier.GetHandles()[2][0].x;
		uint8_t z_ay = bezier.GetHandles()[2][0].y;
		uint8_t z_bx = bezier.GetHandles()[2][1].x;
		uint8_t z_by = bezier.GetHandles()[2][1].y;

		uint8_t r_ax = bezier.GetHandles()[3][0].x;
		uint8_t r_ay = bezier.GetHandles()[3][0].y;
		uint8_t r_bx = bezier.GetHandles()[3][1].x;
		uint8_t r_by = bezier.GetHandles()[3][1].y;

		uint8_t (*mem)[16] = (decltype(mem))interpolation;

		mem[0][0] = x_ax;
		mem[1][0] = mem[0][1] = y_ax;
		mem[2][0] = mem[1][1] = mem[0][2] = z_ax;
		mem[3][0] = mem[2][1] = mem[1][2] = mem[2][1] = r_ax;

		mem[3][1] = mem[2][2] = mem[1][3] = mem[0][4] = x_ay;
		mem[3][2] = mem[2][3] = mem[1][4] = mem[0][5] = y_ay;
		mem[3][3] = mem[2][4] = mem[1][5] = mem[0][6] = z_ay;
		mem[3][4] = mem[2][5] = mem[1][6] = mem[0][7] = r_ay;

		mem[3][5] = mem[2][6] = mem[1][7] = mem[0][8] = x_bx;
		mem[3][6] = mem[2][7] = mem[1][8] = mem[0][9] = y_bx;
		mem[3][7] = mem[2][8] = mem[1][9] = mem[0][10] = z_bx;
		mem[3][8] = mem[2][9] = mem[1][10] = mem[0][11] = r_bx;

		mem[3][9] = mem[2][10] = mem[1][11] = mem[0][12] = x_by;
		mem[3][10] = mem[2][11] = mem[1][12] = mem[0][13] = y_by;
		mem[3][11] = mem[2][12] = mem[1][13] = mem[0][14] = z_by;
		mem[3][12] = mem[2][13] = mem[1][14] = mem[0][15] = r_by;

		mem[3][13] = mem[2][14] = mem[1][15] = 1;
		mem[3][14] = mem[2][15] = 0;
		mem[3][15] = 0;
	}

	void VMDFile::LoadMotionData(const Animation& anim)
	{
		const auto& boneKeyframeMatrix = anim.GetBoneKeyframeMatrix();
		const auto& pmx = anim.GetModel().GetPMXFile();
		for (uint32_t boneIndex = 0; boneIndex < boneKeyframeMatrix.size(); ++boneIndex) {
			const auto& keyframes = boneKeyframeMatrix[boneIndex];
			for (const auto &keyframe : keyframes) {
				//const auto& keyframe = keyframes[keyframeIndex];
				MotionData md = {};
				const std::string& boneName = pmx.GetBoneName(boneIndex);
				LoadName(boneName, sizeof(md.boneName), md.boneName);
				md.frameNumber = keyframe.frame;
				md.position[0] = keyframe.transform.translation.x;
				md.position[1] = keyframe.transform.translation.y;
				md.position[2] = keyframe.transform.translation.z;
				md.rotation[0] = keyframe.transform.rotation.x;
				md.rotation[1] = keyframe.transform.rotation.y;
				md.rotation[2] = keyframe.transform.rotation.z;
				md.rotation[3] = keyframe.transform.rotation.w;
				LoadBezier(keyframe.bezier, md.interpolation);
				m_motionDatas.push_back(md);
			}
		}
	}

	void VMDFile::LoadMorphData(const Animation& anim)
	{
		const auto& morphKeyframeMatrix = anim.GetMorphKeyframeMatrix();
		const auto& pmx = anim.GetModel().GetPMXFile();
		for (uint32_t morphIndex = 0; morphIndex < morphKeyframeMatrix.size(); ++morphIndex) {
			const auto& keyframes = morphKeyframeMatrix[morphIndex];
			for (const auto& keyframe : keyframes) {
				//const auto& keyframe = keyframes[keyframeIndex];
				MorphData md = {};
				const std::string& morphName = pmx.GetMorphName(morphIndex);
				LoadName(morphName, sizeof(md.morphName), md.morphName);
				md.frameNumber = keyframe.frame;
				md.weight = keyframe.weight;
				m_morphDatas.push_back(md);
			}
		}
	}

	void VMDFile::Serialize(const std::filesystem::path& path)
	{
		std::ofstream stream;
		stream.exceptions(std::ofstream::failbit | std::ofstream::badbit);
		stream.open(path, std::ofstream::binary);
		SerializeHeader(stream);
		SerializeMotionData(stream);
		SerializeMorphData(stream);
	}

	void VMDFile::SerializeHeader(std::ofstream& stream)
	{
		/* Must seperate since struct is not packed */
		Write(stream, m_header.magic);
		Write(stream, m_header.modelName);
	}

	void VMDFile::SerializeMotionData(std::ofstream& stream)
	{
		uint32_t count = m_motionDatas.size();
		Write(stream, count);

		for (const auto& md : m_motionDatas) {
			Write(stream, md.boneName);
			Write(stream, md.frameNumber);
			Write(stream, md.position);
			Write(stream, md.rotation);
			Write(stream, md.interpolation);
		}
	}

	void VMDFile::SerializeMorphData(std::ofstream& stream)
	{
		uint32_t count = m_morphDatas.size();
		Write(stream, count);

		for (const auto& md : m_morphDatas) {
			Write(stream, md.morphName);
			Write(stream, md.frameNumber);
			Write(stream, md.weight);
		}
	}
}