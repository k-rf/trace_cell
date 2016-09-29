#include "stdafx.h"

#include <Windows.h>

#include "Timer.hpp"

#pragma comment(lib, "winmm.lib")

using namespace std;

// ============================================================================
// コンストラクタ
// ============================================================================
Timer::Timer()
{
	restart();
}

// ============================================================================
// 計測開始時間を保存
// ============================================================================
void Timer::restart()
{
	start = timeGetTime();
}

// ============================================================================
// restart() からの秒数を返す
// ============================================================================
double Timer::elapsed()
{
	end = timeGetTime();

	return static_cast<double>((end - start));
}
