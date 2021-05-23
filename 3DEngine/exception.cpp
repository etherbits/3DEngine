#include "exception.h"
#include <sstream>
BaseException::BaseException(unsigned int line, const char* fileName) noexcept
	: line(line), fileName(fileName) {}

const char* BaseException::what() const noexcept
{
	std::ostringstream oss;
	oss << GetType() << std::endl
		<< GetOriginString();
	whatBuffer = oss.str();
	return whatBuffer.c_str();
}

const char* BaseException::GetType() const noexcept
{
	return "Base BaseException";
}

const char* BaseException::GetFileName() const noexcept
{
	return fileName;
}

unsigned int BaseException::GetLine() const noexcept
{
	return line;
}


std::string BaseException::GetOriginString() const noexcept
{
	std::ostringstream oss;
	oss << "FILE: " << fileName << std::endl
		<< "LINE: " << line;
	return oss.str();
}