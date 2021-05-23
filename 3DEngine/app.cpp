#include "app.h"

Window* App::pMainWindow = nullptr;
std::vector<Object*> App::pObjects = {new Cube()};
Camera App::camera(Vec3(0, -2.f, 0), Vec3(0, 1.f, 0));


App::App()
{}

int App::Init()
{
	if (const auto exitCode = Window::ProcessMessages()) { return  *exitCode; } // start proccessing messages
	return -1;
}

void App::UpdateFrame()
{
	Mat4 transformMatrix = Mat4::RotateX(.02f) * Mat4::RotateY(.01f) * Mat4::RotateZ(.005f);
	Vec3* vertices = pObjects[0]->GetVertices();
	for (int i = 0; i < pObjects[0]->GetVertexCount(); i++)
	{
		vertices[i] = transformMatrix * vertices[i];
	}
	Face* faces = pObjects[0]->GetFaces();
	for (int i = 0; i < pObjects[0]->GetFaceCount(); i++)
	{
		faces[i].normal = transformMatrix * faces[i].normal;
	}
}

void App::RenderFrame()
{
	pMainWindow->GetGfx()->ClearBuffer();
	for (const Object* object : pObjects)
	{
		Face* pObjectFaces = object->GetFaces();
		for (int i = 0; i < object->GetFaceCount(); i++)
		{
			if (VecAngleBetween(camera.GetPosition() - pObjectFaces[i].GetCenter(), pObjectFaces[i].normal) <= PI/2)
			{
				Vec2 projectedVertices[3] = {};
				for (int j = 0; j < 3; j++)
				{
					Vec3 projectedVertex = camera.GetPosition() - *pObjectFaces[i].vertices[j]; // get projected vector from camera -> vertex vector
					projectedVertex = projectedVertex / projectedVertex.y; // get projected vector from camera -> vertex vector
					projectedVertices[j] = Vec2(projectedVertex.x, projectedVertex.z);
				}
				pMainWindow->GetGfx()->DrawTriangle(projectedVertices[0], projectedVertices[1], projectedVertices[2], { 20, 20, 255 });
				//pMainWindow->GetGfx()->BufferPixel(pMainWindow->GetGfx()->ToScreenSpace(projectedVertices[0]), { 255, 255, 255 });
				//pMainWindow->GetGfx()->BufferPixel(pMainWindow->GetGfx()->ToScreenSpace(projectedVertices[1]), { 255, 255, 255 });
				//pMainWindow->GetGfx()->BufferPixel(pMainWindow->GetGfx()->ToScreenSpace(projectedVertices[2]), {255, 255, 255});
				pMainWindow->GetGfx()->DrawLine(projectedVertices[0], projectedVertices[1], { 0, 127, 255 });
				pMainWindow->GetGfx()->DrawLine(projectedVertices[1], projectedVertices[2], { 0, 127, 255 });
				pMainWindow->GetGfx()->DrawLine(projectedVertices[2], projectedVertices[0], { 0, 127, 255 });
			}
		}
	}
	//pMainWindow->GetGfx()->DrawLineToCursor({ 0,0 }, {255, 0, 40});
}

void App::PresentFrame()
{
	pMainWindow->GetGfx()->PresentBuffer();
}

void App::SetMainWindow(Window* pWindow){ pMainWindow = pWindow; }