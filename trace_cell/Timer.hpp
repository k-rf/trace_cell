#ifndef ___TIMER_HPP
#define ___TIMER_HPP

#include "stdafx.h"

#include <Windows.h>

class Timer
{
	DWORD start;
	DWORD end;

public:
	// �R���X�g���N�^
	Timer();

	// �v���J�n���Ԃ�ۑ�
	void restart();

	// restart() ����̕b����Ԃ�
	double elapsed();
};

#endif
