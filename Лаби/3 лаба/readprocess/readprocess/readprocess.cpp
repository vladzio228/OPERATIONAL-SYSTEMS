#include <iostream>
#include<windows.h>
#include<Shlwapi.h>
#include <tchar.h>
#include <psapi.h>
#include <processthreadsapi.h>
#include <string>
#include <codecvt>
#include <locale>
#pragma warning( disable : 4996 )
#pragma warning( disable : 4700 )
#pragma comment(lib,"Shlwapi")

using namespace std;

HANDLE hProc = NULL;
HWND Window = NULL;

int main()
{
	int Value = 999; //New intended health Value
	DWORD procID;
	char text[] = "Running Process";
	wchar_t wtext[20];
	mbstowcs(wtext, text, strlen(text) + 1);
	LPWSTR ptr = wtext;

	Window = FindWindow(NULL, ptr);

	if (!Window)
	{
		cout << "Window not found" << endl;
		return 1;
	}
	else
	{
		if (!GetWindowThreadProcessId(Window, &procID))
		{
			cerr << "Get window process ID error: " << GetLastError() << endl;
			return 1;
		}
	}

	if (procID != 0)
		hProc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, procID);

	if (!hProc)
	{
		cerr << "Can not open process: " << GetLastError() << endl;
		return 2;
	}
	else
	{
		cout << "Process Found and ready to read" << endl;
	}

	CHAR buffer[sizeof(int)];
	std::cout << "Enter address: \n";
	SIZE_T bytesRead;
	wchar_t* argv = (wchar_t*)malloc(100);
	LONGLONG addr;
	std::wcin >> argv;
	StrToInt64Ex(argv, STIF_SUPPORT_HEX, &addr);
	memset(buffer, 0x0, sizeof(int));;
	while (ReadProcessMemory(hProc, (LPCVOID)addr, (LPVOID)buffer, sizeof(int), &bytesRead))
	{
		Sleep(1000);
		int* valP = reinterpret_cast<int*>(&buffer);
		int val = *valP;
		_tprintf(_T("Successfully read: %d\n"), val);
	}
	CloseHandle(Window);
	CloseHandle(hProc);

	return 0;
}