// Author: Kele // http://keledev.pl // kele@keledev.pl
// License: beerware

#include "WinConsole.h"
#include <io.h>
#include <fcntl.h>
BOOL WinConsole::Init() 
{
	AllocConsole();
	AttachConsole(GetCurrentProcessId());
	if((hInput = GetStdHandle(STD_INPUT_HANDLE)) == INVALID_HANDLE_VALUE) return 1;
	if((hOutput = GetStdHandle(STD_OUTPUT_HANDLE)) == INVALID_HANDLE_VALUE) return 2;
	if((hError = GetStdHandle(STD_ERROR_HANDLE)) == INVALID_HANDLE_VALUE) return 3;
	if(!SyncHandleWithStdio(stdin, "r", input, _O_TEXT)) return 4;
	if(!SyncHandleWithStdio(stdout, "w", output, _O_TEXT)) return 5;
	if(!SyncHandleWithStdio(stderr, "w", error, _O_TEXT)) return 6;
	if(!Flush()) return 7;
	
	CONSOLE_SCREEN_BUFFER_INFO csbinfo;
	GetConsoleScreenBufferInfo(hOutput, &csbinfo);
	wDefColors = csbinfo.wAttributes;

	return 0;
}
WinConsole::~WinConsole()
{
	FreeConsole();
}

BOOL WinConsole::SyncHandleWithStdio(FILE *pfStdioHandle, const char* pStdioFlags, HandleType handle, int iConsoleFlags)
{
	int iHandle;
	FILE *fp;
	HANDLE handles[3] = {hInput, hOutput, hError};

	if((iHandle = _open_osfhandle((long)handles[handle], iConsoleFlags)) == -1) return 1;
	if(NULL != (fp = _fdopen(iHandle, pStdioFlags))) return 2;
	*pfStdioHandle = *fp;
	if(setvbuf(pfStdioHandle, 0, _IONBF, 0)) return 3;

	return 0;
}
BOOL WinConsole::Attach(DWORD dwProcessID)
{
	return AttachConsole(dwProcessID);
}
int WinConsole::ChangeColor(WORD wColors, bool changeDefColors)
{
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	COORD coord = {0,0};
	DWORD dwAttributesWritten;

	wDefColors = wColors;
	if(!GetConsoleScreenBufferInfo(hOutput, &csbi)) return 1;
	if(!FillConsoleOutputAttribute(hOutput, wColors, csbi.dwSize.X * csbi.dwSize.Y, coord, &dwAttributesWritten)) return 2;
	if(changeDefColors)
		SetColor(wColors);

	return 0;
}
void WinConsole::ClearScreen(WORD wColors)
{
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	COORD coordHome = {0,0};
	DWORD dwWritten;

	GetConsoleScreenBufferInfo(hOutput, &csbi);
	FillConsoleOutputCharacter(hOutput, ' ', csbi.dwSize.X * csbi.dwSize.Y, coordHome, &dwWritten);
	FillConsoleOutputAttribute(hOutput, (wColors) ? wColors : wDefColors,
		csbi.dwSize.X * csbi.dwSize.Y, coordHome, &dwWritten);
	SetCursorPos(coordHome);

}
BOOL WinConsole::Create()
{
	return AllocConsole();
}
HANDLE WinConsole::CreateScreenBuffer(DWORD dwDesiredAcces, DWORD dwShareMode, const SECURITY_ATTRIBUTES* pSecurityAttributes)
{
	return CreateConsoleScreenBuffer(dwDesiredAcces, dwShareMode, pSecurityAttributes, CONSOLE_TEXTMODE_BUFFER, NULL);
}
BOOL WinConsole::FillOutputAttribute(WORD wAttribute, DWORD dwLenght, COORD WriteCoord, DWORD *pdwNumberOfAttrWritten)
{
	DWORD tmp;
	
	if(pdwNumberOfAttrWritten == 0) pdwNumberOfAttrWritten = &tmp;
	return FillConsoleOutputAttribute(hOutput, wAttribute, dwLenght, WriteCoord, pdwNumberOfAttrWritten);
}
BOOL WinConsole::FillOutputCharacter(CHAR cCharacter, DWORD dwLenght, COORD WriteCoord, DWORD *pdwNumberOfCharsWritten)
{
	DWORD tmp;

	if(pdwNumberOfCharsWritten == 0) pdwNumberOfCharsWritten = &tmp;
	return FillConsoleOutputCharacter(hOutput, cCharacter, dwLenght, WriteCoord, pdwNumberOfCharsWritten);
}
BOOL WinConsole::Flush()
{
	return FlushConsoleInputBuffer(hInput);
}
char WinConsole::GetChar()
{
	DWORD dwCharsRead;
	char retValue;
	DWORD mode;

	GetMode(input, &mode);
	SetMode(input, ENABLE_PROCESSED_INPUT);
	ReadConsole(hInput, &retValue, 1, &dwCharsRead, 0);
	SetMode(input, mode);
	return retValue;
}
BOOL WinConsole::GetCursorInfo(CONSOLE_CURSOR_INFO *pConsoleCursorInfo)
{
	return GetConsoleCursorInfo(hOutput, pConsoleCursorInfo);
}
BOOL WinConsole::GetDisplayMode(DWORD *pdwDisplayMode)
{
	return GetConsoleDisplayMode(pdwDisplayMode);
}
HANDLE WinConsole::GetHandle(DWORD dwStdHandle)
{
	return GetStdHandle(dwStdHandle);
}
WORD WinConsole::GetKey()
{
	DWORD dwNumberOfEvents, dwEventsRead;
	INPUT_RECORD *buffer;

	GetNumberOfConsoleInputEvents(hInput, &dwNumberOfEvents);
	if(dwNumberOfEvents == 0) return 0;
	buffer = new INPUT_RECORD [dwNumberOfEvents];
	ReadConsoleInput(hInput, buffer, dwNumberOfEvents, &dwEventsRead);
	for(unsigned i = 0; i < dwNumberOfEvents; i++)
		if( (buffer[i].EventType == KEY_EVENT) && buffer[i].Event.KeyEvent.bKeyDown)
		{
			WORD retValue = buffer[i].Event.KeyEvent.wVirtualKeyCode;
			delete [] buffer;
			return retValue;
		}
	delete [] buffer;
	return 0;
}
BOOL WinConsole::GetMode(HandleType handle, DWORD *pdwMode)
{
	HANDLE handles[3] = {hInput, hOutput, hError};

	return GetConsoleMode(handles[handle], pdwMode);
}
BOOL WinConsole::GetNumOfInputEvents(DWORD *pdwNumberOfEvents)
{
	return GetNumberOfConsoleInputEvents(hOutput, pdwNumberOfEvents);
}
DWORD WinConsole::GetProcessList(DWORD *pdwProcessList, DWORD dwProcessCount)
{
	return GetConsoleProcessList(pdwProcessList, dwProcessCount);
}
BOOL WinConsole::GetScreenBufferInfo(CONSOLE_SCREEN_BUFFER_INFO *pConsoleScreenBufferInfo)
{
	return GetConsoleScreenBufferInfo(hOutput, pConsoleScreenBufferInfo);
}
DWORD WinConsole::GetTitle(char *sTitle, DWORD dwSize)
{
	return GetConsoleTitle(sTitle, dwSize);
}
HWND WinConsole::GetWindow()
{
	return GetConsoleWindow();
}
bool WinConsole::KeyHit()
{
	DWORD dwNumOfEvents, dwEventsRead;
	INPUT_RECORD *buffer;

	GetNumberOfConsoleInputEvents(hInput, &dwNumOfEvents);
	buffer = new INPUT_RECORD [dwNumOfEvents];
	PeekConsoleInput(hInput, buffer, dwNumOfEvents, &dwEventsRead);
	for(unsigned i = 0; i < dwNumOfEvents; i++)
		if((buffer[i].EventType == KEY_EVENT) && buffer[i].Event.KeyEvent.bKeyDown)
		{
			delete [] buffer;
			return true;
		}
		delete [] buffer;
	return false;
}
BOOL WinConsole::PeekInput(INPUT_RECORD * pBuffer, DWORD dwLenght, DWORD *pdwNumOfEventsRead)
{
	return PeekConsoleInput(hInput, pBuffer, dwLenght, pdwNumOfEventsRead);
}
BOOL WinConsole::Read(VOID * pBuffer, DWORD dwNumOfCharsToRead, DWORD *pdwNumOfCharsRead)
{
	return ReadConsole(hInput, pBuffer, dwNumOfCharsToRead, pdwNumOfCharsRead, 0);
}
BOOL WinConsole::ReadInput(INPUT_RECORD *pBuffer, DWORD dwLenght, DWORD *pdwNumOfEventsRead)
{
	return ReadConsoleInput(hInput, pBuffer, dwLenght, pdwNumOfEventsRead);
}
BOOL WinConsole::ReadOutput(CHAR_INFO *pBuffer, COORD BufferSize, COORD BufferCoord, SMALL_RECT *pReadRegion)
{
	return ReadConsoleOutput(hOutput, pBuffer, BufferSize, BufferCoord, pReadRegion);
}
BOOL WinConsole::ReadOutputAttribute(WORD *pwAttribute, DWORD dwLenght, COORD ReadCoord, DWORD *pdwNumberOfAttrsRead)
{
	return ReadConsoleOutputAttribute(hOutput, pwAttribute, dwLenght, ReadCoord, pdwNumberOfAttrsRead);
}
BOOL WinConsole::ReadOutputCharacter(char *pBuffer, DWORD dwLenght, COORD ReadCoord, DWORD *pdwNumberOfCharsRead)
{
	return ReadConsoleOutputCharacter(hOutput, pBuffer, dwLenght, ReadCoord, pdwNumberOfCharsRead);
}
BOOL WinConsole::SetTitle(char *sConsoleTitle)
{
	return SetConsoleTitle(sConsoleTitle);
}
BOOL WinConsole::SetColor(WORD wColors)
{
	wDefColors = wColors;
	return SetConsoleTextAttribute(hOutput, wColors);
}
BOOL WinConsole::SetCursorPos(SHORT x, SHORT y)
{
	COORD coord = {x,y};

	return SetConsoleCursorPosition(hOutput, coord);
}
BOOL WinConsole::SetCursorPos(COORD coord)
{
	return SetConsoleCursorPosition(hOutput, coord);
}
BOOL WinConsole::SetCursorVisibility(BOOL bVisible)
{
	CONSOLE_CURSOR_INFO ccinfo;

	GetConsoleCursorInfo(hOutput, &ccinfo);
	ccinfo.bVisible = bVisible;
	return SetConsoleCursorInfo(hOutput, &ccinfo);
}
BOOL WinConsole::SetDisplayMode(DWORD dwMode)
{
	COORD coord;

	return SetConsoleDisplayMode(hOutput, dwMode, &coord);
}
BOOL WinConsole::SetMode(int handle, DWORD dwMode)
{
	HANDLE handles[3] = {hInput, hOutput, hError};
	return SetConsoleMode(handles[handle], dwMode);
}
BOOL WinConsole::SetScreenBufferSize(COORD Size)
{
	return SetConsoleScreenBufferSize(hOutput, Size);
}
BOOL WinConsole::SetWindowInfo(BOOL bAbsolute, const SMALL_RECT *pConsoleWindow)
{
	return SetConsoleWindowInfo(hOutput, bAbsolute, pConsoleWindow);
}
BOOL WinConsole::Write(const char *pBuffer, DWORD dwNumberOfCharsToWrite, DWORD *pdwNumberOfCharsWritten)
{
	DWORD tmp;

	if(pdwNumberOfCharsWritten == 0) pdwNumberOfCharsWritten = &tmp;
	if(dwNumberOfCharsToWrite == 0)
	{
		int i;
		for(i = 0; pBuffer[i]; i++);
		dwNumberOfCharsToWrite = i;
	}
	return WriteConsole(hOutput, pBuffer, dwNumberOfCharsToWrite, pdwNumberOfCharsWritten, NULL);
}
BOOL WinConsole::WriteInput(const INPUT_RECORD *pBuffer, DWORD dwLenght, DWORD *pdwNumOfEventsWritten)
{
	DWORD tmp;

	if(pdwNumOfEventsWritten == 0) pdwNumOfEventsWritten = &tmp;
	return WriteConsoleInput(hInput, pBuffer, dwLenght, pdwNumOfEventsWritten);
}
BOOL WinConsole::WriteOutput(const CHAR_INFO *pBuffer, COORD BufferSize, COORD BufferCoord, SMALL_RECT *pWriteRegion)
{
	return WriteConsoleOutput(hOutput, pBuffer, BufferSize, BufferCoord, pWriteRegion);
}
BOOL WinConsole::WriteOutputAttribute(const WORD *pwAttribute, DWORD dwLenght, COORD WriteCoord, DWORD *pdwNumberOfAttrsWritten)
{
	DWORD tmp;

	if(pdwNumberOfAttrsWritten == 0) pdwNumberOfAttrsWritten = &tmp;
	return WriteConsoleOutputAttribute(hOutput, pwAttribute, dwLenght, WriteCoord, pdwNumberOfAttrsWritten);
}
BOOL WinConsole::WriteOutputCharacter(char *pCharacters, DWORD dwLenght, COORD WriteCoord, DWORD *pdwNumberOfCharsWritten)
{
	DWORD tmp;

	if(pdwNumberOfCharsWritten == 0) pdwNumberOfCharsWritten = &tmp;
	return WriteConsoleOutputCharacter(hOutput, pCharacters, dwLenght, WriteCoord, pdwNumberOfCharsWritten);
}
