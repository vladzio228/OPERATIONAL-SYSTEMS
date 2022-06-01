#include <iostream>
#include <windows.h> 
#include <ctime>
#pragma warning(disable:4996)

int main()
{
	char text[] = "Running Process";
	wchar_t wtext[20];
	mbstowcs(wtext, text, strlen(text) + 1);
	LPWSTR ptr = wtext;
	SetConsoleTitle(ptr);
	int Run = 100;
	int start = clock();
	std::cout << "Address of Running Process: " << &Run << '\n';
	while (Run >= 0)
	{
		if (clock() - start > 1000)
		{
			start = clock();
			std::cout << "Running: " << Run << '\n';
			Run--;
		}
	}
	return 0;
}
