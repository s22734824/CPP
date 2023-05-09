#pragma once
#include <string>
#include <vector>
#include <functional>
typedef std::function<void()> RegistryFunction;
class StringWriter
{
public:
	StringWriter(std::string ObjectName, std::string IniValue);
	std::string GetValue();
	void SetValue(std::string Value);
	void WriteAllValue();
private:
	const std::string _FILE_PATH = "Config.txt";
	static std::vector<std::string> _allObject;
	static std::vector<RegistryFunction> _AllObjectRegistry;
	const std::vector<std::string> split(const std::string& str, const std::string& pattern);
	bool fileExit(std::string path);
	std::string _Name;
	std::string _Value;
	void RegistryFunction();
	//void WriteAllValue();
};