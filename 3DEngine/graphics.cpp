#include "graphics.h"
#include "window.h"

#define GFX_THROW_ON_FAIL(hResult) if(FAILED(hResult)){ throw Graphics::GraphicsException(__LINE__, __FILE__, hResult); }
#define GFX_EXCEPT(hResult) throw Graphics::GraphicsException(__LINE__, __FILE__, hResult)

Graphics::Graphics(HWND hWnd)
    : hWnd(hWnd)
{
    frontHDC = GetDC(hWnd);

    // get width and height of client area
    RECT clientRect;
    GetClientRect(hWnd, &clientRect);
    width = clientRect.right;
    height = clientRect.bottom;

    // create back buffer
    pBuffer = new RGB[static_cast<long long>(width) * height * 4];

    RGB black = { 0, 0, 0 };
    std::fill(pBuffer, pBuffer + (static_cast<long long>(width) * height), black);

    BITMAPINFOHEADER bmiHeader = {};
    bmiHeader.biSize = sizeof(bmiHeader);
    bmiHeader.biWidth = width;
    bmiHeader.biHeight = height;
    bmiHeader.biPlanes = 1;
    bmiHeader.biBitCount = 24;
    bmiHeader.biCompression = BI_RGB;

    bmi.bmiHeader = bmiHeader;
}

Graphics::~Graphics()
{
    ReleaseDC(hWnd, frontHDC);
    delete[] pBuffer;
}

void Graphics::Resize() // only gets called on WM_SIZE message
{
    RECT clientRect;
    GetClientRect(hWnd, &clientRect);
    width = clientRect.right;
    height = clientRect.bottom;

    delete[] pBuffer;
    pBuffer = new RGB[static_cast<long long>(width) * height * 4];
    RGB black = { 0, 0, 0 };
    std::fill(pBuffer, pBuffer + (static_cast<long long>(width) * height), black);

    bmi.bmiHeader.biWidth = width;
    bmi.bmiHeader.biHeight = height;
}

void Graphics::ClearBuffer()
{
    RGB black = { 0, 0, 0 };
    std::fill(pBuffer, pBuffer + (static_cast<long long>(width) * height), black);
}

void Graphics::PresentBuffer() const
{
    int res = SetDIBitsToDevice(
        frontHDC,
        0,
        0,
        bmi.bmiHeader.biWidth,
        bmi.bmiHeader.biHeight,
        0,
        0,
        0,
        bmi.bmiHeader.biWidth,
        (void*)pBuffer,
        &bmi,
        DIB_RGB_COLORS
    );
}

void Graphics::BufferPixel(Vec2& target, RGB color)
{
    Clamp(target);
    pBuffer[width * height - static_cast<int>(target.y) * width + static_cast<int>(target.x)] = color;
}

void Graphics::BufferPixel(int x, int y, RGB color)
{
    Vec2 target(x,y);
    Clamp(target);
    pBuffer[width * height - static_cast<int>(target.y) * width + static_cast<int>(target.x)] = color;
}

void Graphics::DrawLine(const Vec2 &v0, const Vec2 &v1, RGB color)
{
    Vec2 cv0 = ToScreenSpace(v0);
    Vec2 cv1 = ToScreenSpace(v1);
    int dx = cv1.x - cv0.x;
    int dy = cv1.y - cv0.y;

    int ix = dx > 0 ? 1 : -1;
    int iy = dy > 0 ? 1 : -1;


    dx = abs(dx);
    dy = abs(dy);
    int maxLen = std::max(dx, dy);

    // remainder used to decide if y should be updated at x 
    float remainder = maxLen / 2;

    int x = cv0.x;
    int y = cv0.y;
    if (dx > dy)
    {
        for (int I = 0; I < maxLen; I++)
        {
            BufferPixel(x, y, color);
            
            x += ix; 
            remainder += dy;
            if (remainder >= dx)
            {
                y += iy;
                remainder -= dx;
            }
        }
    }
    else
    {
        for (int I = 0; I < maxLen; I++)
        {
            BufferPixel(x, y, color);

            y += iy;
            remainder += dx;
            if (remainder >= dy)
            {
                x += ix;
                remainder -= dy;
            }
        }
    }

    // old
    /*Vec2 cv0 = ToScreenSpace(v0);
Vec2 cv1 = ToScreenSpace(v1);
int slope, error;
char yStep = cv1.y > cv0.y ? 1 : -1,
    xStep = cv1.x > cv0.x ? 1 : -1;
if (std::abs(cv1.x - cv0.x) > std::abs(cv1.y - cv0.y))
{
    slope = 2 * (cv1.y - cv0.y) * yStep;
    error = slope - (cv1.x - cv0.x) * xStep;
    for (int x = cv0.x, y = cv0.y, end = (int)cv1.x; x != end; x+=xStep)
    {
        error += slope;

        if (error >= 0)
        {
            y+=yStep;
            error -= 2 * (cv1.x - cv0.x) * xStep;
        }
        Vec2 target(x, y);
        BufferPixel(target, color);
    }
}
else
{
    slope = 2 * (cv1.x - cv0.x) * xStep;
    error = slope - (cv1.y - cv0.y) * yStep;
    for (int x = cv0.x, y = cv0.y, end = (int)cv1.y; y != end; y+=yStep)
    {
        error += slope;

        if (error >= 0)
        {
            x += xStep;
            error -= 2 * (cv1.y - cv0.y) * yStep;
        }
        Vec2 target(x, y);
        BufferPixel(target, color);
    }
}*/

}

void Graphics::DrawLineToCursor(const Vec2& v0, RGB color)
{
    POINT cursorPos;
    GetCursorPos(&cursorPos);
    ScreenToClient(hWnd, &cursorPos);
    Vec2 cursorVec2(cursorPos.x, cursorPos.y);
    BufferPixel(cursorVec2, { 0, 0, 255 });
    ToCentralSpace(cursorVec2);
    DrawLine(v0, cursorVec2, color);
    ToScreenSpace(cursorVec2);
    BufferPixel(cursorVec2, {0, 255, 0});
}

void Graphics::DrawTriangle(const Vec2& v0, const Vec2& v1, const Vec2& v2, RGB color)
{
    const Vec2 cv0 = ToScreenSpace(v0),  cv1 = ToScreenSpace(v1),  cv2 = ToScreenSpace(v2);
    const Vec2 *pv0 = &cv0, *pv1 = &cv1, *pv2 = &cv2;
    if (pv0->y > pv1->y) std::swap(pv0, pv1);
    if (pv1->y > pv2->y) std::swap(pv1, pv2);
    if (pv0->y > pv1->y) std::swap(pv0, pv1);

    if (pv0->y == pv1->y)
    {
        if (pv0->x > pv1->x) std::swap(pv0, pv1);
        DrawTriangleBaseTop(*pv0, *pv1, *pv2, color);
    }
    else if (pv1->y == pv2->y)
    {
        if (pv1->x > pv2->x) std::swap(pv1, pv2);
        DrawTriangleBaseBottom(*pv0, *pv1, *pv2, color);
    }
    else
    {
        const float alphaSplit = (pv1->y - pv0->y) / (pv2->y - pv0->y);
        const Vec2 splitVertex = *pv0 + (*pv2 - *pv0) * alphaSplit;
        if (splitVertex.x > pv1->x) // major right
        {
            DrawTriangleBaseTop(*pv1, splitVertex, *pv2, color);
            DrawTriangleBaseBottom(*pv0, *pv1, splitVertex, color);
        }
        else // major left
        {
            DrawTriangleBaseTop(splitVertex, *pv1, *pv2, color);
            DrawTriangleBaseBottom(*pv0, splitVertex, *pv1, color);
        }
    }
}

void Graphics::DrawTriangleBaseTop(const Vec2& v0, const Vec2& v1, const Vec2& v2, RGB color)
{
    const float slope0 = (v2.x - v0.x) / (v2.y - v0.y),
        slope1 = (v2.x - v1.x) / (v2.y - v1.y);

    const int yStart = static_cast<int>(ceil(static_cast<double>(v0.y) - .5f)),
        yEnd = static_cast<int>(ceil(static_cast<double>(v2.y) - .5f));

    for (int y = yStart; y < yEnd; y++)
    {
        const int xStart = ceilf((slope0 * (y + .5f - v0.y) + v0.x) - .5f),
            xEnd = ceilf((slope1 * (y + .5f - v0.y) + v1.x) - .5f);

        for (int x = xStart; x < xEnd; x++)
        {
            Vec2 target(x, y);
            BufferPixel(target, color);
        }
    }
}

void Graphics::DrawTriangleBaseBottom(const Vec2& v0, const Vec2& v1, const Vec2& v2, RGB color)
{
    const float slope0 = (v1.x - v0.x) / (v1.y - v0.y),
        slope1 = (v2.x - v0.x) / (v2.y - v0.y);

    const int yStart = static_cast<int>(ceil(static_cast<double>(v0.y) - .5f)),
        yEnd = static_cast<int>(ceil(static_cast<double>(v2.y) - .5f));

    for (int y = yStart; y < yEnd; y++)
    {
        const int xStart = ceilf((slope0 * (y + .5f - v0.y) + v0.x) - .5f),
            xEnd = ceilf((slope1 * (y + .5f - v0.y) + v0.x) - .5f);

        for (int x = xStart; x < xEnd; x++)
        {
            Vec2 target(x, y);
            BufferPixel(target, color);
        }
    }
}

Vec2& Graphics::ToCentralSpace(Vec2& vec) const
{
    vec.x = (vec.x - width/2) / (width / 2);
    vec.y = -(vec.y - height / 2) / (height / 2);
    return vec;
}

Vec2& Graphics::ToScreenSpace(Vec2& vec) const
{
    vec.x = vec.x * width / 2 + width / 2;
    vec.y = -vec.y * height / 2 + height / 2;
    return vec;
}

const Vec2& Graphics::ToScreenSpace(const Vec2& vec) const
{
    return { vec.x * width / 2 + width / 2, 
            -vec.y * height / 2 + height / 2};
}

Vec2& Graphics::Clamp(Vec2& vec) const
{
    if (vec.x < 0.f)
    {
        vec.x = 0;
    }
    else if (vec.x >= width)
    {
        vec.x = width - 1;
    }
    if (vec.y < 0.f)
    {
        vec.y = 0;
    }
    else if (vec.y >= height)
    {
        vec.y = height - 1;
    }
    return vec;
}

const Vec2& Graphics::Clamp(const Vec2& vec) const
{
    float x = vec.x, y=vec.y;
    if (vec.x < 0.f)
    {
        x = 0;
    }
    else if (vec.x >= width)
    {
        x = width - 1;
    }
    if (vec.y < 0.f)
    {
        y = 0;
    }
    else if (vec.y >= height)
    {
        y = height - 1;
    }
    return {x, y};
}

RGB::RGB()
    : b(0), g(0), r(0)
{}

RGB::RGB(const unsigned char r, const unsigned char g, const unsigned char b) // invert order of colors because gdi uses b-g-r order
    : r(r), g(g), b(b)
{}