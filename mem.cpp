#include "mem.h"

void mem::patchEx( HANDLE hProcess, BYTE* dst, BYTE* src, uintptr_t size )
{
	DWORD oldProtect{};
	VirtualProtectEx( hProcess, dst, size, PAGE_EXECUTE_READWRITE, &oldProtect );
	
	WriteProcessMemory( hProcess, dst, src, size, nullptr );

	VirtualProtectEx( hProcess, dst, size, oldProtect, &oldProtect );

}

void mem::nopEx( HANDLE hProcess, BYTE* dst, unsigned int size )
{
	BYTE* nopArray{ new BYTE[size] };

	// 0x90 = nop 
	memset( nopArray, 0x90, size );

	patchEx( hProcess, dst, nopArray, size );
	delete[] nopArray;
}