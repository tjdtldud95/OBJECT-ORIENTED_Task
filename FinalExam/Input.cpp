#include "Input.h"
#include <vector>

Input* Input::Instance = nullptr;
std::vector<WORD> Input::KeyCodeTable{
		VK_SPACE, VK_LEFT, VK_RIGHT, VK_UP, VK_DOWN,
		VK_ESCAPE, VK_RETURN,
		0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F, 0x50,
		0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5A
};


bool Input::getKeyDown(WORD virtualKeyCode) const
{
	// TODO: NOT FULLY IMPLEMENTED YET
	return getKey(virtualKeyCode);
}

bool Input::getKey(WORD virtualKeyCode) const
{
	if (cNumRead == 0) return false;

	for (int i = 0; i < (int)cNumRead; i++)
	{
		if (irInBuf[i].EventType != KEY_EVENT) continue;

		if (irInBuf[i].Event.KeyEvent.wVirtualKeyCode == virtualKeyCode &&
			irInBuf[i].Event.KeyEvent.bKeyDown == TRUE) {
			return true;
		}
	}
	return false;
}

bool Input::getKeyUp(WORD virtualKeyCode) const 
{
	if (cNumRead == 0) return false;

	for (int i = 0; i < (int)cNumRead; i++)
	{
		if (irInBuf[i].EventType != KEY_EVENT) continue;

		if (irInBuf[i].Event.KeyEvent.wVirtualKeyCode == virtualKeyCode &&
			irInBuf[i].Event.KeyEvent.bKeyDown == FALSE) {
			return true;
		}
	}
	return false;
}

bool Input::getMouseButtonDown(unsigned short number) 
{
	for (int i = 0; i < (int)cNumRead; i++) {
		if (irInBuf[i].EventType != MOUSE_EVENT) continue;

		auto& mouseEvent = irInBuf[i].Event.MouseEvent;
		if (mouseEvent.dwEventFlags != 0) continue;

		if ((number == 0 && mouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED) ||
			(number == 1 && mouseEvent.dwButtonState == RIGHTMOST_BUTTON_PRESSED) ||
			(number == 2 &&
				(mouseEvent.dwButtonState == (FROM_LEFT_1ST_BUTTON_PRESSED | RIGHTMOST_BUTTON_PRESSED)
					|| mouseEvent.dwButtonState == FROM_LEFT_2ND_BUTTON_PRESSED))) {
			mousePosition = mouseEvent.dwMousePosition;
			return true;
		}
	}
	return false;
}

void Input::keyEventProc(KEY_EVENT_RECORD ker)
{
	if (ker.bKeyDown) {

	}
	switch (ker.wVirtualKeyCode) {

	case VK_LBUTTON:
		break;
	case VK_BACK:
		break;
	case VK_RETURN:
		break;
	case VK_LEFT:
		break;
	case VK_UP:
		break;
	default:
		break;
	}

	Borland::Gotoxy(0, 0);
}

void Input::mouseEventProc(MOUSE_EVENT_RECORD mer)
{

#ifndef MOUSE_HWHEELED
#define MOUSE_HWHEELED 0x0008
#endif


	switch (mer.dwEventFlags)
	{
	case 0:
		if (mer.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED)
		{
			
		}
		else if (mer.dwButtonState == RIGHTMOST_BUTTON_PRESSED)
		{
			
		}
		else
		{
			
		}
		break;
	case DOUBLE_CLICK:
		
		break;
	case MOUSE_HWHEELED:
		
		break;
	case MOUSE_MOVED:
	
		break;
	case MOUSE_WHEELED:
	
		break;
	default:
	
		break;
	}

}

void Input::resizeEventProc(WINDOW_BUFFER_SIZE_RECORD wbsr)
{
}

void Input::errorExit(const char* lpszMessage)
{

	// Restore input mode on exit.

	SetConsoleMode(hStdin, fdwSaveOldMode);

	ExitProcess(0);
}
