#include "window.h"
#include "app.h"
#include <sstream>

Window::Window(unsigned short width, unsigned short height, const wchar_t* name)
{
    RECT clientRect;
    clientRect.left = 0;
    clientRect.top = 0;
    clientRect.right = width;
    clientRect.bottom = height;

    AdjustWindowRect(&clientRect, WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX, FALSE);
    hWnd = CreateWindowW(WindowClass::GetClassName(), name, WS_SYSMENU | WS_MAXIMIZEBOX | WS_MINIMIZEBOX, CW_USEDEFAULT, CW_USEDEFAULT, 
        clientRect.right - clientRect.left, clientRect.bottom - clientRect.top, nullptr, nullptr, WindowClass::GetHInstance(), this);
    if (hWnd == nullptr)
    {
        throw WIN_LAST_EXCEPT();
    }
    ShowWindow(hWnd, SW_SHOW);
    
    gfx = std::make_unique<Graphics>(hWnd);

}

Window::~Window() { DestroyWindow(hWnd); }

Graphics* Window::GetGfx() const
{
    if (gfx.get() != nullptr)
    {
        return gfx.get();
    }
    throw WIN_EXCEPT("graphics is not initialized");
}

std::optional<int> Window::ProcessMessages()
{
    bool bGotMsg;
    MSG  msg;
    msg.message = WM_NULL;
    PeekMessage(&msg, NULL, 0U, 0U, PM_NOREMOVE);

    while (WM_QUIT != msg.message)
    {
        // Process window events.
        // Use PeekMessage() so we can use idle time to render the scene. 
        bGotMsg = (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE) != 0);

        if (bGotMsg)
        {
            // Translate and dispatch the message
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {
            // Update the scene.
            App::UpdateFrame();

            // Render frames during idle time (when no messages are waiting).
            App::RenderFrame();

            // Present the frame to the screen.
            App::PresentFrame();
        }
    }
    return msg.wParam;
}

LRESULT CALLBACK Window::WindowProcSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept 
{
    if (msg == WM_NCCREATE) 
    {
        const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
        Window* const pWnd = static_cast<Window*>(pCreate->lpCreateParams);
        SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));
        SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(WindowProcRedirect));
        return  pWnd->WindowProcRedirect(hWnd, msg, wParam, lParam);
    }

    return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT CALLBACK Window::WindowProcRedirect(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept 
{
    Window* const pWnd = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
    return pWnd->WindowProc(hWnd, msg, wParam, lParam);
}

LRESULT Window::WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept 
{
    switch (msg) 
    {
    case WM_CLOSE:
        PostQuitMessage(0);
        return 0;
    case WM_SIZE:
        if(gfx != nullptr)
        gfx.get()->Resize();
    }
    return DefWindowProc(hWnd, msg, wParam, lParam);
}

// V--V window class V--V

Window::WindowClass Window::WindowClass::classInstance;

Window::WindowClass::WindowClass()
    : hInstance(GetModuleHandle(nullptr))
{
    WNDCLASS wc = { 0 };
    wc.style = CS_OWNDC; // allow unique device context for each window in the class
    wc.lpfnWndProc = WindowProcSetup;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = nullptr;
    wc.hCursor = nullptr;
    wc.hbrBackground = nullptr;
    wc.lpszMenuName = nullptr;
    wc.lpszClassName = className;
    RegisterClass(&wc);
}

Window::WindowClass::~WindowClass() { UnregisterClass(className, hInstance); }

const wchar_t* Window::WindowClass::GetClassName() { return className; }

HINSTANCE Window::WindowClass::GetHInstance() { return classInstance.hInstance; }

// V--V window exception V--V
std::string Window::WinException::TranslateErrorCode(HRESULT hResult) noexcept
{
    char* pMsgBuffer = nullptr;
    DWORD msgLen = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, 
        nullptr, hResult, MAKELANGID(LANG_NEUTRAL , SUBLANG_DEFAULT), reinterpret_cast<LPSTR>(&pMsgBuffer), 0, nullptr);
    if (msgLen == 0) {
        return "Unidentified error code";
    }
    std::string errorString = pMsgBuffer;
    LocalFree(pMsgBuffer);
    return errorString;
}

Window::WinException::WinException(unsigned int line, const char* fileName, HRESULT hResult) noexcept
    : BaseException(line, fileName), hResult(hResult), description(nullptr) {}

Window::WinException::WinException(unsigned int line, const char* fileName, const char* description) noexcept
    : BaseException(line, fileName), hResult(E_FAIL), description(description) {}

const char* Window::WinException::GetType() const noexcept{ return "Window Exception"; }

const char* Window::WinException::what() const noexcept
{
    std::ostringstream oss;
    if (description == nullptr)
    {
        oss << GetType() << std::endl
            << "ERROR CODE: " << GetErrorCode() << std::endl
            << "ERROR MESSAGE: " << GetErrorString() << std::endl
            << GetOriginString();
    }
    else 
    {
        oss << GetType() << std::endl
            << "ERROR MESSAGE: " << description << std::endl
            << GetOriginString();
    }
    whatBuffer = oss.str();
    return whatBuffer.c_str();
    
}

HRESULT Window::WinException::GetErrorCode() const noexcept{ return hResult; }

std::string Window::WinException::GetErrorString() const noexcept{ return TranslateErrorCode(hResult); }