﻿#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>

#include "GameFramework/Debug.h"
#include "GameFramework/Game.h"
//Hàm main
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	Game::GetInstance()->Run();

	return 0;
}