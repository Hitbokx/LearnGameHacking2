#include <Windows.h>
#include <iostream>
#include <string>
#include <vector>

using namespace std; 

uintptr_t findAdd( HANDLE hProc, uintptr_t ptr, std::vector<uintptr_t> offsets )
{
	uintptr_t addr{ ptr };

	for ( int i{ 0 }; i < offsets.size( ); ++i )
	{
		ReadProcessMemory( hProc, (LPCVOID)addr, &addr, sizeof( addr ), nullptr );
		addr += offsets[i];
	}

	return addr;
}

#define CHAR_ARRAY_SIZE 128

int main( ) {
	int varInt( 123456 );
	std::string varString( "DefaultString" );
	char arrChar[CHAR_ARRAY_SIZE] = "Long char array right there ->";
	int* ptr2int( &varInt );
	int** ptr2ptr( &ptr2int );
	int*** ptr2ptr2( &ptr2ptr );
	uintptr_t pVarString{ (uintptr_t)&varString };
	uintptr_t pArrChar{ (uintptr_t)&arrChar };
	char arrToReplaceWith[150] = "You are HACKED!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!";
	


	int intRead{ 0 };
	int intToWrite{ 987654 };
	char array[128]{};
	std::string stringToReplaceWith{ "I am COOL!" };

		cout << "Process ID: " << dec << GetCurrentProcessId( ) << endl;
		cout << endl;
		cout << "varInt       (0x" << hex << uppercase << (uintptr_t)&varInt << ") = " << dec << varInt << endl;
		cout << "varString    (0x" << hex << uppercase << (uintptr_t)&varString << ") = " << varString << endl;
		cout << "arrChar[" << dec << CHAR_ARRAY_SIZE << "] (0x" << hex << uppercase << (uintptr_t)&arrChar << ") = " << arrChar << endl;
		cout << endl;
		cout << "ptr2int      (0x" << hex << uppercase << (uintptr_t)&ptr2int << ") = 0x" << ptr2int << endl;
		cout << "ptr2ptr      (0x" << hex << uppercase << (uintptr_t)&ptr2ptr << ") = 0x" << ptr2ptr << endl;
		cout << "ptr2ptr2     (0x" << hex << uppercase << (uintptr_t)&ptr2ptr2 << ") = 0x" << ptr2ptr2 << endl;
		cout << endl;
		cout << endl << "---------------------------------------------------" << endl << endl;

	HANDLE hProcess{ OpenProcess( PROCESS_ALL_ACCESS, FALSE, GetCurrentProcessId( ) ) };
	if ( hProcess == NULL )
	{
		std::cout << "OpenProcess failed. GetLastError = " << std::dec << GetLastError( ) << '\n';
		system( "pause" );
		return EXIT_FAILURE;
	}
 
	uintptr_t pIntReadAddr{ findAdd( hProcess, (uintptr_t)&ptr2ptr2, {0x0,0x0,0x0} ) };
	BOOL rpmReturn{ ReadProcessMemory( hProcess, (LPCVOID)pIntReadAddr, &intRead, sizeof( intRead ), NULL ) };
	if ( rpmReturn == FALSE )
	{
		std::cout << "ReadProcessMemory failed. GetLastError = " << std::dec << GetLastError( ) << '\n';
		system( "pause" );
		return EXIT_FAILURE;
	}
	std::cout << "intRead = " << std::dec << intRead << '\n' << '\n';


	uintptr_t pIntToWriteAdd{ findAdd( hProcess, (uintptr_t)&ptr2ptr2, {0x0,0x0,0x0} ) };
	BOOL wpmReturn{ WriteProcessMemory( hProcess, (LPVOID)pIntToWriteAdd, &intToWrite, sizeof( intToWrite ), nullptr ) };
	if ( wpmReturn == FALSE )
	{
		std::cout << "WriteProcessMemory failed. GetLastError = " << GetLastError( ) << '\n';
		system( "pause" );
		return EXIT_FAILURE;
	}
	std::cout << "Overwritten successfully.\n";
	std::cout << "varInt = " << std::dec << varInt << '\n' << '\n';

	
	BOOL wpmReturn2{ WriteProcessMemory( hProcess, (LPVOID)pVarString, &stringToReplaceWith, sizeof( stringToReplaceWith ), NULL ) };
	if ( wpmReturn2 == FALSE )
	{
		std::cout << "WriteProcessMemory to STRING failed. GetLastError = " << GetLastError( ) << '\n';
		system( "pause" );
		return EXIT_FAILURE;
	}
	std::cout << "varString = " << varString << '\n' << '\n';


	BOOL wpmReturn3{ WriteProcessMemory( hProcess, (LPVOID)pArrChar, &arrToReplaceWith, sizeof( arrToReplaceWith ),nullptr ) };
	if ( wpmReturn3 == FALSE )
	{
		std::cout << "WriteProcessMemory to ARRAY failed. GetLastError = " << GetLastError( ) << '\n';
		system( "pause" );
		return EXIT_FAILURE;
	}
	std::cout << "arrChar = " << arrChar << '\n' << '\n';
	std::cout << "Press ENTER to quit.";

	getchar( );

	CloseHandle( hProcess );

	return EXIT_SUCCESS;
}