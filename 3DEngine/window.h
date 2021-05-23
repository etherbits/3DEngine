#pragma once
#include "win.h"
#include "exception.h"
#include "graphics.h"
#include <optional>
#include <memory>

class Window 
{
public:
	Window(unsigned short width, unsigned short height, const wchar_t* name);
	~Window();
	Window(const Window&) = delete;
	static std::optional<int> ProcessMessages();
	Graphics* GetGfx() const;
private:
	HWND hWnd;
	std::unique_ptr<Graphics> gfx;
	static LRESULT CALLBACK WindowProcSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	static LRESULT CALLBACK WindowProcRedirect(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	LRESULT WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
private:

	class WindowClass 
	{
	public:
		static  const wchar_t* GetClassName();
		static HINSTANCE GetHInstance();
	private:
		WindowClass();
		~WindowClass();
		WindowClass(const WindowClass&) = delete;
		WindowClass& operator=(const WindowClass&) = delete;
		static constexpr const wchar_t className[] = L"3D Engine";
		static WindowClass classInstance;
		HINSTANCE hInstance;
	};
public:
	class WinException : public BaseException {
	public:
		WinException(unsigned int line, const char* fileName, HRESULT hResult) noexcept;
		WinException(unsigned int line, const char* fileName, const char* description) noexcept;
		const char* what() const noexcept override;
		const char* GetType() const noexcept override;
		HRESULT GetErrorCode() const noexcept;
		std::string GetErrorString() const noexcept;
	private:
		static std::string TranslateErrorCode(HRESULT hResult) noexcept;
		HRESULT hResult;
		const char* description;
	};
};

// exception helper macro 
#define WIN_EXCEPT( hResult ) Window::WinException(__LINE__, __FILE__, hResult)
#define WIN_LAST_EXCEPT( hResult ) Window::WinException(__LINE__, __FILE__, GetLastError())