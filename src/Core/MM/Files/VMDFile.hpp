#pragma once

#include "File.hpp"

namespace mm
{
	struct VMDParseError : public std::runtime_error
	{
		VMDParseError(const char* what = "") : std::runtime_error(what) {}
	};

	class Animation;

	class VMDFile : public File
	{
		static constexpr char MAGIC[] = "Vocaloid Motion Data 0002";
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
		/* Load from file */
		explicit VMDFile(const std::filesystem::path& path);
		/* Load from Animation */
		explicit VMDFile(const Animation& anim);
		void Serialize(const std::filesystem::path& path);
		virtual std::string GetName() const override { return m_path.filename().string(); }
		virtual std::filesystem::path GetPath() const override { return m_path; }
		const std::vector<MotionData>& GetMotionDatas() const { return m_motionDatas; }
		const std::vector<MorphData>& GetMorphDatas() const { return m_morphDatas; }

	private:
		void ParseHeader(std::ifstream& stream);
		void ParseMotionData(std::ifstream& stream);
		void ParseMorphData(std::ifstream& stream);

		void LoadHeader(const Animation& anim);
		void LoadMotionData(const Animation& anim);
		void LoadMorphData(const Animation& anim);

		void SerializeHeader(std::ofstream& stream);
		void SerializeMotionData(std::ofstream& stream);
		void SerializeMorphData(std::ofstream& stream);

	private:
		//std::filesystem::path m_path;
		Header                  m_header;
		std::vector<MotionData> m_motionDatas;
		std::vector<MorphData>  m_morphDatas;
	};

}

