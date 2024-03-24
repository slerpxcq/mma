#pragma once

namespace mm
{
	struct VMDParseError : public std::runtime_error
	{
		VMDParseError(const char* what = "") : std::runtime_error(what) {}
	};

	class VMDFile
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
		const std::vector<MotionData>& GetMotionDatas() const { return m_motionDatas; }
		const std::vector<MorphData>& GetMorphDatas() const { return m_morphDatas; }

	private:
		void ParseHeader(std::ifstream& stream);
		void ParseMotionData(std::ifstream& stream);
		void ParseMorphData(std::ifstream& stream);

	private:
		Header                  m_header;
		std::vector<MotionData> m_motionDatas;
		std::vector<MorphData>  m_morphDatas;
	};

}

