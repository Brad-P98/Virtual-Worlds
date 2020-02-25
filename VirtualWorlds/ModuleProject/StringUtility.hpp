#pragma once
#include <GL/gl3w.h>
#include <iostream>
#include <fstream>
#include <ostream>
#include <set>
#include <vector>

class StringUtility {
	
public:

	enum class StringResult : uint8_t { S_OKAY, S_FILE_NOT_FOUND, S_BUFFER_ALLOC_ERROR };

	// Split a path into it's component parts - separated by characters in delimiters, and return a vector of the component parts
	static std::vector<std::string> splitPath(const std::string& path, const std::set<char>& delimiters);

	static std::string loadStringFromFile(const std::string& filePath);
};
