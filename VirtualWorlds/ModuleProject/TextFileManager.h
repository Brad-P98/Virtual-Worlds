#pragma once
#include <sstream>		//Includes string

//Handle reading and writing to text files
class TextFileManager
{
public:

	static void writeToFile(std::string fileName, std::string message);

	static void writeToFile(std::string fileName, float number);

	static void writeFloatStatToFile(std::string fileName, std::string statName, float stat);

	static int extractIntFromFile(std::string fileName);


	static void clearFile(std::string fileName);
private:

	static std::string fpPrefix;
};

