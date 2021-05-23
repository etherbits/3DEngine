#include "window.h"
#include "app.h"
#include "obj.h"

INT CALLBACK WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow) 
{
    try
    {
        Object obj(Vec3(0, 0, 0));
        App app = App();
        Window wnd(1400, 1400, L"3D Engine Main");
        app.SetMainWindow(&wnd);
        return app.Init();
    }
    catch (const BaseException& e)
    {
        MessageBoxA(nullptr, e.what(), e.GetType(), MB_OK | MB_ICONEXCLAMATION);
    }
    catch (const std::exception& e)
    {
        MessageBoxA(nullptr, e.what(), "Standard Exception", MB_OK | MB_ICONEXCLAMATION);
    }
    catch (...)
    {
        MessageBoxA(nullptr, "Something went wrong", "Unknown Exception", MB_OK | MB_ICONEXCLAMATION);
    }
}