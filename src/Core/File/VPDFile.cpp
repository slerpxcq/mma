#include "CorePch.hpp"
#include "VPDFile.hpp"

#include "Common/Locale.hpp"
#include "Text.hpp"

#include <regex>

namespace mm
{

static void MatchAll(DynArray<String>& result, const String& str, const std::regex& e)
{
	std::regex_token_iterator<std::string::const_iterator> end;
	decltype(end) it{ str.cbegin(), str.cend(), e };
	while (it != end) {
		result.emplace_back(it->str());
		++it;
	}
}

VPDFile::VPDFile(const Path& path) :
	File{ path }
{
	auto src = Text(path);
	auto str = Locale::ShiftJISToUTF8(src.GetString().c_str(),
									  src.GetString().size());

	std::regex nameRegex{ "\\{.*$" };
	/* NOTE: technically -?*\\d+\\.\\d+ is correct, but is extremelly slow 
	         for unknown reason */
	std::regex valRegex{ "-*\\d+\\.\\d+" };
	DynArray<String> names;
	DynArray<String> vals;

	MatchAll(names, str, nameRegex);
	MatchAll(vals, str, valRegex);

	MM_CORE_ASSERT(names.size() * 7 == vals.size());

	for (i32 i = 0; i < names.size(); ++i) {
		f32 tX = std::atof(vals[7 * i + 0].c_str());
		f32 tY = std::atof(vals[7 * i + 1].c_str());
		f32 tZ = std::atof(vals[7 * i + 2].c_str());
		f32 rX = std::atof(vals[7 * i + 3].c_str());
		f32 rY = std::atof(vals[7 * i + 4].c_str());
		f32 rZ = std::atof(vals[7 * i + 5].c_str());
		f32 rW = std::atof(vals[7 * i + 6].c_str());
		String& name = names[i];
		name.erase(name.cbegin());
		m_pose.insert({ name, 
						   Transform{ Vec3{ tX, tY, tZ }, 
						              Quat{ rW, rX, rY, rZ }} });
	}
}

Transform VPDFile::GetTransform(StringView boneName) const
{
	auto it = m_pose.find(String{ boneName });
	return it != m_pose.end() ?
		it->second : Transform{};
}

}
