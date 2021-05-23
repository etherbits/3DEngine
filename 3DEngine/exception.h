#pragma once
#include <exception>
#include <string>

class BaseException : public std::exception {
public:
	BaseException(unsigned int line, const char* fileName) noexcept;
	const char* what() const noexcept override;
	virtual const char* GetType() const noexcept;
	unsigned int GetLine() const noexcept;
	const char* GetFileName() const noexcept;
	std::string GetOriginString() const noexcept;
private:
	unsigned int line;
	const char* fileName;
protected:
	mutable std::string whatBuffer;
};