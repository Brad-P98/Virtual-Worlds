#include "TextFileManager.h"

#include <fstream>
#include<iostream>

std::string TextFileManager::fpPrefix = "Text/";

void TextFileManager::writeToFile(std::string fileName, std::string message)
{

	std::ofstream file;
	file.open(fpPrefix + fileName);
	file << message + "\n";
	file.close();

}

void TextFileManager::writeToFile(std::string fileName, float number)
{
	std::ostringstream ss;
	ss << number;
	std::string s(ss.str());

	std::ofstream file;
	file.open(fpPrefix + fileName);
	file << s;
	file.close();

}

void TextFileManager::writeFloatStatToFile(std::string fileName, std::string statName, float stat)
{
	std::ostringstream ss;
	ss << stat;
	std::string statStr(ss.str());

	std::ofstream file;
	//Append mode
	file.open(fpPrefix + fileName, std::ios_base::app);

	file << statName + ": ";
	file << statStr + "\n";
	file.close();

}

int TextFileManager::extractIntFromFile(std::string fileName)
{
	std::string strFromFile;
	int intFromFile = 0;


	std::ifstream file;
	file.open(fpPrefix + fileName);
	std::getline(file, strFromFile);

	std::stringstream ss(strFromFile);

	if (ss >> intFromFile) return intFromFile;
	//else std::cout << "error extracting int, or nothing in file" << std::endl;

	file.close();
	return -1;
}

void TextFileManager::clearFile(std::string fileName)
{

	std::ofstream file;
	file.open(fpPrefix + fileName);

	file.close();
}
