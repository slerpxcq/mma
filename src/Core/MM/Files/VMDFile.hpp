#pragma once

#include "File.hpp"

namespace mm
{
	struct VMDParseError : public std::runtime_error
	{
		VMDParseError(const char* what = "") : std::runtime_error(what) {}
	};

	class VMDFile : public File
	{
	public:
		struct Header {
			char magic[30];
			char modelName[20];
		};

		struct MotionData {
			char    boneName[15];
			int32_t frameNumber;
			float   position[3];
			float   rotation[4];
			uint8_t interpolation[64];
		};

		struct MorphData {
			char    morphName[15];
			int32_t frameNumber;
			float   weight;
		};

	public:
		explicit VMDFile(const std::filesystem::path& path);
		virtual const std::string& GetName() const override { return m_path.filename().u8string(); }
		virtual const std::filesystem::path& GetPath() const override { return m_path; }
		const std::vector<MotionData>& GetMotionDatas() const { return m_motionDatas; }
		const std::vector<MorphData>& GetMorphDatas() const { return m_morphDatas; }

	private:
		void ParseHeader(std::ifstream& stream);
		void ParseMotionData(std::ifstream& stream);
		void ParseMorphData(std::ifstream& stream);

	private:
		std::filesystem::path m_path;
		Header                  m_header;
		std::vector<MotionData> m_motionDatas;
		std::vector<MorphData>  m_morphDatas;
	};

}

