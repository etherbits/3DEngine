#include "graphics.h"
#include <sstream>

std::string Graphics::GraphicsException::TranslateErrorCode(HRESULT hResult) noexcept
{
    char* pMsgBuffer = nullptr;
    DWORD msgLen = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        nullptr, hResult, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), reinterpret_cast<LPSTR>(&pMsgBuffer), 0, nullptr);
    if (msgLen == 0) {
        return "Unidentified error code";
    }
    std::string errorString = pMsgBuffer;
    LocalFree(pMsgBuffer);
    return errorString;
}

Graphics::GraphicsException::GraphicsException(unsigned int line, const char* fileName, HRESULT hResult) noexcept
    : BaseException(line, fileName), hResult(hResult) {}

const char* Graphics::GraphicsException::GetType() const noexcept { return "Graphics Exception"; }

const char* Graphics::GraphicsException::what() const noexcept
{
    std::ostringstream oss;
    oss << GetType() << std::endl
        << "ERROR CODE: " << GetErrorCode() << std::endl
        << "ERROR MESSAGE: " << GetErrorString() << std::endl
        << GetOriginString();
    whatBuffer = oss.str();
    return whatBuffer.c_str();
}

HRESULT Graphics::GraphicsException::GetErrorCode() const noexcept { return hResult; }

std::string Graphics::GraphicsException::GetErrorString() const noexcept { return TranslateErrorCode(hResult); }