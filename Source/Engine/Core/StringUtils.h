#pragma once
#include <string>

namespace nc
{
	class StringUtils
	{
	private:
	public:
		static std::string ToUpper(const std::string& string);
		static std::string ToLower(const std::string& string);

		static bool IsEqualIgnoreCase(std::string string1, std::string string2);

		static std::string CreateUnique(std::string& string);

	private:
	};
}