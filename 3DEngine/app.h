#pragma once
#include "window.h"
#include "obj.h"
#include "camera.h"
#include <vector>
class App 
{
	static Window* pMainWindow;
	static std::vector<Object*> pObjects;
	static Camera camera;
public:
	App();
	App(const App&) = delete;
	App operator=(const App&) = delete;
	static int Init();
	static void UpdateFrame();
	static void RenderFrame();
	static void PresentFrame();
	void SetMainWindow(Window* pWindow);
};