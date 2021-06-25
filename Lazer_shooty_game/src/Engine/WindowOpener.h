#pragma once
#include <string>


class WindowOpener
{
public :

	WindowOpener();
	~WindowOpener();
	std::string PrintAndOpenStuff();
	std::string PrintAndOpenPng();
	std::string SaveFileStuff();
	static std::string GetFilePathSave(const char* _extension);
	static std::string GetFilepathOpen(const char* _extension);

private:

};

