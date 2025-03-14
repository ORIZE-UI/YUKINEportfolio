#ifndef __DEFINES_H__
#define __DEFINES_H__

#include <assert.h>
#include <Windows.h>
#include <stdarg.h>
#include <stdio.h>

// リソースパス
#define ASSET(path)	"Assets/"path

// タイトル
#ifdef _DEBUG
#define APP_TITLE "Team_203"
#else
#define APP_TITLE "Define.hでタイトルが変更できるので、ゲームのタイトルに変えておくこと"
#endif

// 画面サイズ
#define SCREEN_WIDTH (1460)
#define SCREEN_HEIGHT (850)

// グリッドサイズ
#define DEBUG_GRID_NUM (10) // グリッド中心から端までの線の数
#define DEBUG_GRID_MARGIN (1.0f) // グリッド配置幅(メートル換算)


#endif // __DEFINES_H__