#include "Proc.h"
#include "mem.h"

int main( )
{
	HANDLE hProcess{ 0 };
	uintptr_t pModuleBase{ 0 };
	uintptr_t pLocalPlayer{ 0 };
	uintptr_t localPlayerAddr{ 0 };
	uintptr_t healthAddr{ 0 };
	uintptr_t armourAddr{ 0 };
	uintptr_t grenadeNumberAdd{ 0 };
	uintptr_t teamAddr{ 0 };

	bool bHealth{ false };
	bool bArmour{ 0 };
	bool bAmmo{ false };
	bool bRecoil{ false };
	bool bGrenade{ false };
	bool bOneShotOneKill{ false };

	const int newValue{ 9999 };
	const int HnewValue{ newValue };
	const int ARnewValue{ newValue };
	const int GnewValue{ newValue };

	const wchar_t* procName{ L"ac_client.exe" };
	DWORD procId{ getProcId( procName ) };

	if ( procId )
	{
		hProcess = OpenProcess( PROCESS_ALL_ACCESS, FALSE, procId );

		pModuleBase = getPtrModuleBaseAddress( procId, procName );

		pLocalPlayer = pModuleBase + 0x10F4F4;

		localPlayerAddr = findDMAAddy( hProcess, pLocalPlayer, {0x0} );

		healthAddr = localPlayerAddr + 0xF8;

		armourAddr = localPlayerAddr + 0xFC;

		grenadeNumberAdd = localPlayerAddr + 0x158;

		teamAddr = localPlayerAddr + 0x238;

	}

	else
	{
		std::cout << "Process not found, press ENTER to exit.\n";
		getchar( );
		return EXIT_FAILURE;
	}

	DWORD dwExit{ 0 };

	while ( GetExitCodeProcess( hProcess, &dwExit ) && dwExit == STILL_ACTIVE )
	{
		if ( GetAsyncKeyState( VK_NUMPAD1 ) & 1 )
		{
				if ( bHealth )
				{
					std::cout << "Unlimited Health hack disabled!!\n";
					std::cout << '\n';
					bHealth = !bHealth;
				}
				else
				{
					std::cout << "Unlimited Health hack enabled!!\n";
					std::cout << '\n';
					bHealth = !bHealth;
				}
		}

		if ( GetAsyncKeyState( VK_NUMPAD2 ) & 1 )
		{
				if ( bArmour )
				{
					std::cout << "Unlimited Health hack disabled!!\n";
					std::cout << '\n';
					bArmour = !bArmour;
				}
				else
				{
					std::cout << "Unlimited Armour hack enabled!!\n";
					std::cout << '\n';
					bArmour = !bArmour;
				}
		}

		if ( GetAsyncKeyState( VK_NUMPAD3 ) & 1 )
		{
			bAmmo = !bAmmo;

			if ( bAmmo )
			{
				std::cout << "Increasing Ammo hack enabled!!\n";
				std::cout << '\n';
				// ff 06 = inc[esi];
				mem::patchEx( hProcess, (BYTE*)(pModuleBase + 0x637E9), (BYTE*)"\xFF\x06", 2 );

			}
			else
			{
				std::cout << "Increasing Ammo hack disabled!!\n";
				std::cout << '\n';
				// ff 0E = dec[esi];
				mem::patchEx( hProcess, (BYTE*)(pModuleBase + 0x637E9), (BYTE*)"\xFF\x0E", 2 );
			}
		}

		if ( GetAsyncKeyState( VK_NUMPAD4 ) & 1 )
		{
			bRecoil = !bRecoil;
			if ( bRecoil )
			{
				std::cout << "No Recoil hack enabled!!\n";
				std::cout << '\n';
				mem::nopEx( hProcess, (BYTE*)(pModuleBase + 0x63786), 10 );
			}
			else
			{
				std::cout << "No Recoil hack disabled!!\n";
				std::cout << '\n';
				mem::patchEx( hProcess, (BYTE*)(pModuleBase + 0x63786), (BYTE*)"\x50\x8d\x4c\x24\x1c\x51\x8b\xce\xff\xd2", 10 );
			}
		}

		if ( GetAsyncKeyState( VK_NUMPAD5 ) & 1 )
		{
			bGrenade = !bGrenade;
			if ( bGrenade )
			{
				std::cout << "Unlimited Grenade hack enabled!!\n";
				std::cout << '\n';
				mem::patchEx( hProcess, (BYTE*)grenadeNumberAdd, (BYTE*)&GnewValue, sizeof( GnewValue ) );
				mem::nopEx( hProcess, (BYTE*)(pModuleBase + 0x63378), 2 );
			}
			else
			{
				std::cout << "Unlimited Grenade hack disabled!!\n";
				std::cout << '\n';
				mem::patchEx( hProcess, (BYTE*)(pModuleBase + 0x63378), (BYTE*)"\xFF\x08", 2 );
			}
		}

		if ( GetAsyncKeyState( VK_NUMPAD6 ) & 1 )
		{
			bOneShotOneKill = !bOneShotOneKill;
			if ( bOneShotOneKill )
			{
				std::cout << "One Shot One Kill hack enabled!!\n";
				std::cout << '\n';

				if ( *((int*)teamAddr) == 0 )
				{
					mem::patchEx( hProcess, (BYTE*)(pModuleBase + 0x29D1F), (BYTE*)"\xC7\x43\x04\x00", 4 );
				}
				else
				{
					mem::patchEx( hProcess, (BYTE*)(pModuleBase + 0x29D1F), (BYTE*)"\x29\x7B\x04", 3 );

				}
			}
			else
			{
				std::cout << "One Shot One Kill hack disabled!!\n";
				std::cout << '\n';
				mem::patchEx( hProcess, (BYTE*)(pModuleBase + 0x29D1F), (BYTE*)"\x29\x7B\x04", 3 );
			}
		}

		if ( GetAsyncKeyState( VK_INSERT ) & 1 )
		{
			return 0;
		}

		if ( bHealth )
		{
			mem::patchEx( hProcess, (BYTE*)healthAddr, (BYTE*)&HnewValue, sizeof( HnewValue ) );
		}

		if ( bArmour )
		{
			mem::patchEx( hProcess, (BYTE*)armourAddr, (BYTE*)&ARnewValue, sizeof( ARnewValue ) );
		}


		Sleep( 10 );
	}

	std::cout << "Process not found, press enter to exit.\n";
	getchar( );

	return 0;
}