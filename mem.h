#pragma once
#include <windows.h>
#include <iostream>

namespace mem
{
	void patchEx( HANDLE hProcess, BYTE* dst, BYTE* src, uintptr_t size );

	void nopEx( HANDLE hProcess, BYTE* dst, unsigned int size );
}
