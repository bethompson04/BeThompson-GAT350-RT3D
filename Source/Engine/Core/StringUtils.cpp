#include "StringUtils.h"

namespace nc
{
	std::string StringUtils::ToUpper(const std::string& string)
	{
		std::string string1 = string;
		for (int i = 0; i < string1.size(); i++)
		{
			string1[i] = toupper(string[i]);
		}
		return string1;
	}

	std::string StringUtils::ToLower(const std::string& string)
	{
		std::string string1 = string;
		for (int i = 0; i < string1.size(); i++)
		{
			string1[i] = tolower(string1[i]);
		}
		return string1;
	}

	bool StringUtils::IsEqualIgnoreCase(std::string string1, std::string string2)
	{
		std::string s1 = ToLower(string1);
		std::string s2 = ToLower(string2);

		if (s1 == s2)
		{
			return true;
		}
		return false;
	}


	std::string CreateUnique(const std::string& str)
	{
		static uint32_t unique = 0;

		return str + std::to_string(unique++);
	}
}