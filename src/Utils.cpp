#include "Utils.hpp"

#include <cctype>
#include <cstring>
#include <string>

bool Utils::EndsWith(const std::string& str, const std::string& suffix)
{
    return str.size() >= suffix.size() && !str.compare(str.size() - suffix.size(), suffix.size(), suffix);
}
bool Utils::StartsWith(const char* str, const char* subStr)
{
    return std::strlen(str) >= std::strlen(subStr) && std::strstr(str, subStr) == str;
}
bool Utils::ICompare(const std::string& a, const std::string& b)
{
    return std::equal(a.begin(), a.end(), b.begin(), b.end(), [](char a, char b) {
        return std::tolower(a) == std::tolower(b);
    });
}
const char *Utils::ArgContinuation(const CCommand &args, int from) {
	const char *text;

	if (args.ArgC() == from + 1) {
		text = args[from];
	} else {
		text = args.m_pArgSBuffer + args.m_nArgv0Size;
		if (from > 1) while (isspace(*text)) ++text;
		for (int i = 1; i < from; i++) {
			text += (*text == '"') * 2 + strlen(args[i]);
			while (isspace(*text)) ++text;
		}
	}

	return text;
}
