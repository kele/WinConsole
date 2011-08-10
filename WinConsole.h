// Author: Kele // http://keledev.pl // kele@keledev.pl
// License: beerware

#pragma once
#include <windows.h>
#include <cstdio>

#define FOREGROUND_WHITE FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED
#define BACKGROUND_WHITE BACKGROUND_GREEN | BACKGROUND_BLUE | BACKGROUND_RED


/* Some comments are from Win32 Programmer's reference.
Shame on me ;< */

class WinConsole
{
	/* Console handles */
	HANDLE hInput, hOutput, hError;

	/* Default colors */
	WORD wDefColors;

public:
	enum {input, output, error};

	typedef int HandleType;

	/* Frees console */
	~WinConsole();

	/* Attaches a console from dwProcessID process */
	BOOL Attach(DWORD dwProcessID);
	
	/* Changes colors of text already printed on screen.
	If changeDefColors is true, calls SetColor(wColors) */
	int ChangeColor(WORD wColors, bool changeDefColors = false);
	
	/* Clears the console screen. If wColors is 0, uses 
	wDefColors to paint the screen. Else it uses wColors. */
	void ClearScreen(WORD wColors = 0);
	
	/* Allocates new console. */
	BOOL Create();
	
	/* Creates screen buffer. */
	HANDLE CreateScreenBuffer(DWORD dwDesiredAcces, DWORD dwShareMode, const SECURITY_ATTRIBUTES* pSecurityAttributes = NULL);
	
	/* Fills nLenght cells of screen buffer with wAttribute. */
	BOOL FillOutputAttribute(WORD wAttribute, DWORD nLenght, COORD WriteCoord, DWORD * pdwNumberOfAttrWritten = 0);
	
	/* Fills nLenght cells of screen buffer with cCharacter. */
	BOOL FillOutputCharacter(CHAR cCharacter, DWORD nLenght, COORD WriteCoord, DWORD * pdwNumberOfCharsWritten = 0);
	
	/* Flushes console input. */
	BOOL Flush();
	
	/* Returns character from console input. 
	Function waits until it gets a character. */
	char GetChar();
	
	/* Gets console cursor info. */
	BOOL GetCursorInfo(CONSOLE_CURSOR_INFO *pConsoleCursorInfo);
	
	/* Gets console display mode. */
	BOOL GetDisplayMode(DWORD *dwDisplayMode); 
	
	/* Gets console handle. */
	HANDLE GetHandle(DWORD nStdHandle);
	
	/* If there is a key in input queue, it returns its virtual key code. 
	Else it returns 0.*/
	WORD GetKey();
	
	/* Gets console mode. */
	BOOL GetMode(int handle, DWORD *retValue);
	
	/* Gets number of input events. */
	BOOL GetNumOfInputEvents(DWORD * pdwNumberOfEvents);
	
	/* Dunno. */
	DWORD GetProcessList(DWORD *pdwProcessList, DWORD dwProcessCount);
	
	/* Gets screen buffer info. */
	BOOL GetScreenBufferInfo(CONSOLE_SCREEN_BUFFER_INFO *pConsoleScreenBufferInfo);
	
	/* Gets console title. */
	DWORD GetTitle(char *sTitle, DWORD dwSize);
	
	/* Returns a handle to console window. */
	HWND GetWindow();
	
	/* Initializes WinConsole (Creates or attaches a new one,
	synchronizes handles with stdio, flushes input, 
	sets default textcolor to white */
	BOOL Init(); 
	
	/* If key is hit, return true. */
	bool KeyHit();
	
	/* Reads data from the console input buffer without removing it from the buffer. */
	BOOL PeekInput(INPUT_RECORD * pBuffer, DWORD dwLenght, DWORD *dwNumOfEventsRead);
	
	/* Reads character input from the console input buffer. */
	BOOL Read(VOID * pBuffer, DWORD dwNumOfCharsToRead, DWORD *dwNumOfCharsRead);
	
	/* Reads data from the console input buffer. */
	BOOL ReadInput(INPUT_RECORD * pBuffer, DWORD dwLenght, DWORD * dwNumOfEventsRead);
	
	/* Reads character and color attribute data from a 
	rectangular block of character cells in a console 
	screen buffer, and the function writes the data to 
	a rectangular block at a specified location in the
	destination buffer. */
	BOOL ReadOutput(CHAR_INFO *pBuffer, COORD BufferSize, COORD BufferCoord, SMALL_RECT *pReadRegion);
	
	BOOL ReadOutputAttribute(WORD *pwAttribute, DWORD nLenght, COORD ReadCoord, DWORD *pdwNumberOfAttrsRead);
	
	BOOL ReadOutputCharacter(char* pBuffer, DWORD dwLenght, COORD ReadCoord, DWORD *pdwNumberOfCharsRead);
	
	/* Sets screen buffer size. */
	BOOL SetScreenBufferSize(COORD Size);
	
	/* Sets console title. */
	BOOL SetTitle(char *sConsoleTitle);
	
	/* Sets console colors. */
	BOOL SetColor(WORD wColors);
	
	/* Sets cursor position. */
	BOOL SetCursorPos(SHORT x, SHORT y);
	
	/* Sets cursor position. */
	BOOL SetCursorPos(COORD coord);
	
	/* Sets cursor visibility. */
	BOOL SetCursorVisibility(BOOL bVisible);
	
	/* Sets display mode. */
	BOOL SetDisplayMode(DWORD dwMode);
	
	/* Sets console mode. */
	BOOL SetMode(int handle, DWORD dwMode);
	
	/* Sets console window info. */
	BOOL SetWindowInfo(BOOL bAbsolute, const SMALL_RECT* pConsoleWindow);
	
	/* Synchronizes WinConsole handles with stdio (called by Init) */
	int SyncHandleWithStdio (FILE *pfStdioHandle, const char* pStdioFlags, HandleType handle, int iConsoleFlags);
	
	/* Writes text from pBuffer. */
	BOOL Write(const char* pBuffer, DWORD dwNumberOfCharsToWrite = 0, DWORD *pdwNumberOfCharsWritten = 0);
	
	/* Writes data to input. */
	BOOL WriteInput(const INPUT_RECORD *pBuffer, DWORD dwLenght, DWORD *pdwNumOfEventsWritten = 0);
	
	/* Writes data. */
	BOOL WriteOutput(const CHAR_INFO* pBuffer, COORD BufferSize, COORD BufferCoord, SMALL_RECT *pWriteRegion);

	BOOL WriteOutputAttribute(const WORD* pwAttribute, DWORD dwLenght, COORD WriteCoord, DWORD *pdwNumberOfAttrsWritten = 0);
	
	BOOL WriteOutputCharacter(char* pCharacters, DWORD dwLenght, COORD WriteCoord, DWORD *pdwNumberOfCharsWritten = 0);
};