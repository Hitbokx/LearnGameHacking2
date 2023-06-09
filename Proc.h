#pragma once

#include <vector>
#include <windows.h>
#include <TlHelp32.h>
#include <iostream>


DWORD getProcId( const wchar_t* procName );

uintptr_t getPtrModuleBaseAddress( DWORD procId, const wchar_t* modName );

uintptr_t findDMAAddy( HANDLE hProc, uintptr_t ptr, std::vector<uintptr_t>offsets );