#pragma once
#include "win.h"
#include "math.h"
#include "exception.h"
#undef RGB

struct RGB
{
    unsigned char b;
    unsigned char g;
    unsigned char r;
    RGB();
    RGB(const unsigned char r, const unsigned char g, const unsigned char b);
};

class Graphics 
{
    HWND hWnd;
    HDC frontHDC;
    unsigned short width;
    unsigned short height;

    RGB* pBuffer;
    BITMAPINFO bmi;
public:
    Graphics(HWND hWnd);
    Graphics(const Graphics&) = delete;
    Graphics operator=(const Graphics&) = delete;
    ~Graphics();
    void Resize();
    void ClearBuffer();
    void PresentBuffer() const;
    void BufferPixel(Vec2& target, RGB color);
    void BufferPixel(int x, int y, RGB color);
    void DrawLine(const Vec2& p0, const Vec2& p1, RGB color);
    void DrawLineToCursor(const Vec2& v0, RGB color);
    void DrawTriangle(const Vec2& p0, const Vec2& p1, const Vec2& p2, RGB color);
    void DrawTriangleBaseTop(const Vec2& v0, const Vec2& v1, const Vec2& v2, RGB color);
    void DrawTriangleBaseBottom(const Vec2& v0, const Vec2& v1, const Vec2& v2, RGB color);
    const Vec2& Clamp(const Vec2 &vec) const;
    Vec2& Clamp(Vec2& vec) const;
    Vec2& ToCentralSpace(Vec2 &vec) const;
    const Vec2& ToScreenSpace(const Vec2& vec) const;
    Vec2& ToScreenSpace(Vec2 &vec) const;
private:
    class GraphicsException : public BaseException
    {
    public:
        GraphicsException(unsigned int line, const char* fileName, HRESULT hResult) noexcept;
        const char* what() const noexcept override;
        const char* GetType() const noexcept override;
        HRESULT GetErrorCode() const noexcept;
        std::string GetErrorString() const noexcept;
    private:
        static std::string TranslateErrorCode(HRESULT hResult) noexcept;
        HRESULT hResult;
    };
};