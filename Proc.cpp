#include "Proc.h"

// used DWORD because the procId in Windows API is DWORD
DWORD getProcId( const wchar_t* procName )
{
	DWORD procId{ 0 };
	HANDLE hSnap{ CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, NULL ) };

	if ( hSnap != INVALID_HANDLE_VALUE )
	{
		PROCESSENTRY32 procEntry{};
		procEntry.dwSize = sizeof( procEntry );

		if ( Process32First( hSnap, &procEntry ))
		{
			do
			{
				if ( !_wcsicmp( procEntry.szExeFile, procName ) )
				{
					procId = procEntry.th32ProcessID;
					break;
				}


			} while ( Process32Next( hSnap, &procEntry ));
		}
	}
	CloseHandle( hSnap );
	return procId;
}

uintptr_t getPtrModuleBaseAddress( DWORD procId, const wchar_t* modName )
{
	// initialize to zero for error checking
	uintptr_t modBaseAddr{ 0 };

	// get a handle to a snapshot of all modules
	HANDLE hSnap{ CreateToolhelp32Snapshot( TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, procId ) };

	// check if its valid
	if ( hSnap != INVALID_HANDLE_VALUE )
	{
		// this struct holds the actual module information
		MODULEENTRY32 modEntry{};

		// this is required for the function to work
		modEntry.dwSize = sizeof( modEntry );

		// if a module exists, get it's entry
		if ( Module32First( hSnap, &modEntry ) )
		{
			// loop through the modules
			do
			{
				// compare the module name against ours
				if ( !_wcsicmp( modEntry.szModule, modName ) )
				{
					// copy the base address and break out of loop
					modBaseAddr = (uintptr_t)modEntry.modBaseAddr;
					break;
				}
				// each iteration we grab the next module entry
			} while ( Module32Next( hSnap, &modEntry ) );
		}
	}

	// free the handle
	CloseHandle( hSnap );
	return modBaseAddr;
}

uintptr_t findDMAAddy( HANDLE hProc, uintptr_t ptr, std::vector<uintptr_t>offsets )
{
	uintptr_t addr{ ptr };
	for ( unsigned int i{ 0 }; i < offsets.size( ); ++i )
	{
		ReadProcessMemory( hProc, (BYTE*)addr, &addr, sizeof( addr ), nullptr );
		addr += offsets[i];
	}
	return addr;
}