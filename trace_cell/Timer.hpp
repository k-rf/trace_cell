#ifndef ___TIMER_HPP
#define ___TIMER_HPP

#include "stdafx.h"

#include <Windows.h>

class Timer
{
	DWORD start;
	DWORD end;

public:
	// コンストラクタ
	Timer();

	// 計測開始時間を保存
	void restart();

	// restart() からの秒数を返す
	double elapsed();
};

#endif
