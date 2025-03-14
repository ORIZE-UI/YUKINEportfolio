#ifndef __MAIN_H__
#define __MAIN_H__

#include <Windows.h>
#include "sceneManager.h"

HRESULT Init(HWND hWnd, UINT width, UINT height);
void Uninit();
void Update(float tick);
void Draw();
SceneManager*	GetSceneMng(void);

#endif