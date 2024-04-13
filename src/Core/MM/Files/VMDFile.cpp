#include "mmpch.hpp"
#include "VMDFile.hpp"

#include "Core/Utility/Stream.hpp"

namespace mm
{
	void VMDFile::ParseHeader(std::ifstream& stream)
	{
		constexpr char magic[] = "Vocaloid Motion Data 0002";

		Read(stream, m_header.magic);
		if (memcmp(m_header.magic, magic, sizeof(magic)))
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
}