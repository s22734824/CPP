#include "StringWiter.h"
#include "string"
#include <iostream>
#include <fstream>
#include <vector>
#include <regex>

StringWriter::StringWriter(std::string ObjectName, std::string IniValue)//can not inline 
{
	_Name = ObjectName;
	_AllObjectRegistry.push_back(std::bind(&StringWriter::RegistryFunction, this));
	if (fileExit(_FILE_PATH))
	{
		std::ifstream file;
		std::string line;
		file.open(_FILE_PATH);
		bool match = false;
		if (file.is_open())
		{
			std::regex regex_pattern(_Name+"(.*)");
			while (getline(file, line))
			{
				if (std::regex_match(line, regex_pattern))
				{
					std::vector<std::string> rec = split(line, "=");
					if (!rec.empty())
					{
						match = true;
						_Value = rec.back();
					}
				}
			}
			if (!match)
			{
				_Value = IniValue;
				WriteAllValue();
			}
		}
	}
	else
	{
		_Value = IniValue;
		WriteAllValue();
	}
}

inline void StringWriter::RegistryFunction()
{
	_allObject.push_back(_Name + "=" + _Value);
}

void StringWriter::WriteAllValue()
{
	_allObject.clear();
	for (auto& Function : _AllObjectRegistry)
	{
		Function();
	}
	std::ofstream file;
	file.open(_FILE_PATH);
	for (std::string& object : _allObject)
	{
		file << object << std::endl;
	}
}

bool StringWriter::fileExit(std::string path)
{
	std::ifstream f(path.c_str());
	return f.good();
}

const std::vector<std::string> StringWriter::split(const std::string& str, const std::string& pattern) {
	std::vector<std::string> result;
	std::string::size_type begin, end;

	end = str.find(pattern);
	begin = 0;

	while (end != std::string::npos) {
		if (end - begin != 0) {
			result.push_back(str.substr(begin, end - begin));
		}
		begin = end + pattern.size();
		end = str.find(pattern, begin);
	}

	if (begin != str.length()) {
		result.push_back(str.substr(begin));
	}
	return result;
}

std::string StringWriter::GetValue()
{
	return _Value;
}
void StringWriter::SetValue(std::string value)
{
	_Value = value;
	WriteAllValue();
}
std::vector<RegistryFunction> StringWriter::_AllObjectRegistry = {};
std::vector<std::string> StringWriter::_allObject = {};