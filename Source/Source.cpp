#include <iostream>
#include "../CompEngine/CompEngine.h"
#include "../CompEngine/CompEngine.cpp"
using namespace std;
using namespace CompEngine;

int main()
{
	EngineCore engine;
	engine.StartGame();
	//engine.Find("Name");
}
/*	POINT op;
HWND hWnd = GetConsoleWindow();
HDC hDC = GetDC(hWnd);
SelectObject(hDC, GetStockObject(WHITE_PEN));

MoveToEx(hDC, 50, 50, &op);
LineTo(hDC, 100, 200);

ReleaseDC(hWnd, hDC);
std::cin.get();
return 0;*/