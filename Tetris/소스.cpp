#define _CRT_SECURE_NO_WARNINGS

#include<iostream>
#include<Windows.h>
#include<conio.h>
#include<string>
#include"Utils.h"

class Screen;
class Block;
class Input;
class BlockManager;

class Screen {
private:
	int		width; // visible width
	int		height;
	char**  canvas;

	// constructor (생성자 함수) 메모리공간상에 적재되는 순간 호출되는
	Screen(int width = 10, int height = 10)
		: width(width), height(height)
	{
		if (this->width <= 0)
		{
			this->width = 10;
		}

		if (this->height <= 0) {
			this->height = 10;
		}

		canvas = (char**)malloc(sizeof(char*) * height);
		for (int i = 0;i < height;i++)
		{
			canvas[i] = (char*)malloc(sizeof(char) * width + 1);
		}
	}
	// destructor (소멸자 함수) 메모리공간상에서 없어지는 순간 호출되는 함수
	~Screen()
	{
		for (int i = 0;i < height;i++)
		{
			free(canvas[i]);
		}
		free(canvas);
		width = 0; height = 0;
	}


public:
	static Screen *instance;

	static Screen* getInstance()
	{
		if (instance == nullptr)
			instance = new Screen(10,20);

		return instance;
	}

	int getWidth()
	{
		return width;
	}
	int getHeigt()
	{
		return height;
	}

	void clear()
	{
		for (int i = 0;i < height;i++)
		{
			memset(canvas[i], ' ', width+1);
			canvas[i][0] = 5;
			canvas[i][width+1] = 5;
			canvas[i][width+2] = '\0';
			if (i == 0 || i == height - 1)
			{
				memset(canvas[i], 6, width+1);
			}
		}

		canvas[0][0] = 1;
		canvas[0][width+1] = 2;
		canvas[height - 1][0] = 3;
		canvas[height - 1][width+1] = 4;
	}

	void render()
	{
		Borland::gotoxy(0, 0);
		for (int i = 0;i < height;i++)
		{
			printf("%s\n", canvas[i]);
		}
	}

	void draw(const Position& pos,const char faceblocknum)
	{
		canvas[pos.y][pos.x] = faceblocknum;
	}

	bool isEmpty(const int x,const int y)
	{
		for (int i = -1;i < 2;i++)
		{
			for (int j = -1;j < 2;j++)
			{
				if (canvas[y + i][x + j] == 127)continue;

				if (canvas[y + i][x + j] != ' ') return false;
			}
		}

		return true;
	}
};

class Input {
	DWORD cNumRead, fdwMode, i;
	INPUT_RECORD irInBuf[128];
	int counter;
	HANDLE hStdin;
	DWORD fdwSaveOldMode;
	char blankChars[80];

	void errorExit(const char*);
	void keyEventProc(KEY_EVENT_RECORD);
	void mouseEventProc(MOUSE_EVENT_RECORD);
	void resizeEventProc(WINDOW_BUFFER_SIZE_RECORD);

	static Input* Instance;

	Input()
	{
		memset(blankChars, ' ', 80);
		blankChars[79] = '\0';

		hStdin = GetStdHandle(STD_INPUT_HANDLE);
		if (hStdin == INVALID_HANDLE_VALUE)
			errorExit("GetStdHandle");
		if (!GetConsoleMode(hStdin, &fdwSaveOldMode))
			errorExit("GetConsoleMode");
		/*
			   Step-1:
			   Disable 'Quick Edit Mode' option programmatically
		 */
		fdwMode = ENABLE_EXTENDED_FLAGS;
		if (!SetConsoleMode(hStdin, fdwMode))
			errorExit("SetConsoleMode");
		/*
		   Step-2:
		   Enable the window and mouse input events,
		   after you have already applied that 'ENABLE_EXTENDED_FLAGS'
		   to disable 'Quick Edit Mode'
		*/
		fdwMode = ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT;
		if (!SetConsoleMode(hStdin, fdwMode))
			errorExit("SetConsoleMode");

	}

	~Input() {
		SetConsoleMode(hStdin, fdwSaveOldMode);
	}

public:

	static Input* GetInstance()
	{
		if (Instance == nullptr) {
			Instance = new Input();
		}
		return Instance;
	}

	void readInputs()
	{
		if (!GetNumberOfConsoleInputEvents(hStdin, &cNumRead)) {
			cNumRead = 0;
			return;
		}
		if (cNumRead == 0) return;

		Borland::gotoxy(0, 22);
		printf("number of inputs %d\n", cNumRead);

		if (!ReadConsoleInput(
			hStdin,      // input buffer handle
			irInBuf,     // buffer to read into
			128,         // size of read buffer
			&cNumRead)) // number of records read
			errorExit("ReadConsoleInput");

		Borland::gotoxy(0, 0);
	}
	bool getKeyDown(WORD virtualKeyCode);
	bool getKey(WORD virtualKeyCode);
	bool getKeyUp(WORD virtualKeyCode);
};

class Block 
{
private:
	Position   pos;
	Input*     input;
	Screen*    screen;

	char face[3][3] = { 127 ,127,127,
						 ' ' ,' ',127,
						 ' ' ,' ',127 };

public:
	Block() : input (Input::GetInstance() ) , screen(Screen :: getInstance())
	{
		pos.x = 5;
		pos.y = 2;
	}

	void draw()
	{
		for (int i = -1;i <= 1;i++)
		{
			for (int j = -1;j <= 1;j++)
			{
				Position drawpos = this->getPos();
				drawpos.y -= i;
				drawpos.x -= j;
				screen->draw(drawpos, getBlockfacenum(i + 1, j + 1));
			}
		}
	}

	void update()
	{
		if (input->getKey(VK_LEFT)) {
			if (pos.x <= 2) return;
			pos.x = (pos.x - 1) % (screen->getWidth());
		}
		if (input->getKey(VK_RIGHT)) {
			if (pos.x >= (screen->getWidth() - 1)) return;
			pos.x = (pos.x + 1) % (screen->getWidth());
		}
		if (input->getKey(VK_UP))
		{
			this->rotateLeftBlockface();
		}

		//one by one
		bool touched = screen->isEmpty(pos.x,pos.y+1);
		if (touched)
			pos.y++;
		
		if (pos.y == 17)
		{
			this->setBlockface('X');
		}
			
	}

	void setBlockface(char blockface)
	{
		for (int i = 0;i < 3;i++)
		{
			for (int j = 0;j < 3;j++)
			{
				if(face[i][j] == 127)
					face[i][j] = blockface;
			}
		}
	}

	void setBlockface(char blockface[][3])
	{
		for (int i = 0;i < 3;i++)
		{
			for (int j = 0;j < 3;j++)
			{
				face[i][j] = blockface[i][j];
			}
		}
	}

	char getBlockfacenum(int hei,int wid)
	{
		return face[hei][wid];
	}

	Position getPos()
	{
		return this->pos;
	}

	void rotateLeftBlockface()
	{
		char tmp[3][3];
		
		for (int i = 0;i < 3;i++)
		{
			for (int j = 0;j < 3;j++)
			{
				tmp[j][3 - i - 1] = this->face[i][j];
			}
		}
		
		this->setBlockface(tmp);
	}

	
};

class BlockManager
{

};

Screen* Screen::instance = nullptr;
Input* Input::Instance = nullptr;

int main()
{
	Screen *screen = Screen::getInstance();
	Block *b = new Block();
	Input* input = Input::GetInstance();

	while (1)
	{
		screen->clear();
		b->draw();

		input->readInputs();

		b->update();
		
		screen->render();	
		Sleep(100);
	}

	return 0;
}


void Input::errorExit(const char* lpszMessage)
{
	fprintf(stderr, "%s\n", lpszMessage);

	// Restore input mode on exit.

	SetConsoleMode(hStdin, fdwSaveOldMode);

	ExitProcess(0);
}

bool Input::getKeyDown(WORD virtualKeyCode)
{
	// TODO: NOT FULLY IMPLEMENTED YET
	return getKey(virtualKeyCode);
}

bool Input::getKey(WORD virtualKeyCode)
{
	if (cNumRead == 0) return false;

	for (int i = 0; i < cNumRead; i++)
	{
		if (irInBuf[i].EventType != KEY_EVENT) continue;

		if (irInBuf[i].Event.KeyEvent.wVirtualKeyCode == virtualKeyCode &&
			irInBuf[i].Event.KeyEvent.bKeyDown == TRUE) {
			return true;
		}
	}
	return false;
}

bool Input::getKeyUp(WORD virtualKeyCode)
{
	if (cNumRead == 0) return false;

	for (int i = 0; i < cNumRead; i++)
	{
		if (irInBuf[i].EventType != KEY_EVENT) continue;

		if (irInBuf[i].Event.KeyEvent.wVirtualKeyCode == virtualKeyCode &&
			irInBuf[i].Event.KeyEvent.bKeyDown == FALSE) {
			return true;
		}
	}
	return false;
}

void Input::keyEventProc(KEY_EVENT_RECORD ker)
{
	Borland::gotoxy(0, 11);
	printf("%s\r", blankChars);
	switch (ker.wVirtualKeyCode) {
	case VK_LBUTTON:
		printf("left button ");
		break;
	case VK_BACK:
		printf("back space");
		break;
	case VK_RETURN:
		printf("enter key");
		break;
	case VK_LEFT:
		printf("arrow left");
		break;
	case VK_UP:
		printf("arrow up");
		break;
	default:
		if (ker.wVirtualKeyCode >= 0x30 && ker.wVirtualKeyCode <= 0x39)
			printf("Key event: %c ", ker.wVirtualKeyCode - 0x30 + '0');
		else printf("Key event: %c ", ker.wVirtualKeyCode - 0x41 + 'A');
		break;
	}

	Borland::gotoxy(0, 0);
}

void Input::mouseEventProc(MOUSE_EVENT_RECORD mer)
{
	Borland::gotoxy(0, 12);
	printf("%s\r", blankChars);
#ifndef MOUSE_HWHEELED
#define MOUSE_HWHEELED 0x0008
#endif
	printf("Mouse event: ");

	switch (mer.dwEventFlags)
	{
	case 0:
		if (mer.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED)
		{
			printf("left button press %d %d\n", mer.dwMousePosition.X, mer.dwMousePosition.Y);
		}
		else if (mer.dwButtonState == RIGHTMOST_BUTTON_PRESSED)
		{
			printf("right button press \n");
		}
		else
		{
			printf("button press\n");
		}
		break;
	case DOUBLE_CLICK:
		printf("double click\n");
		break;
	case MOUSE_HWHEELED:
		printf("horizontal mouse wheel\n");
		break;
	case MOUSE_MOVED:
		printf("mouse moved %d %d\n", mer.dwMousePosition.X, mer.dwMousePosition.Y);
		break;
	case MOUSE_WHEELED:
		printf("vertical mouse wheel\n");
		break;
	default:
		printf("unknown\n");
		break;
	}
	Borland::gotoxy(0, 0);
}

void Input::resizeEventProc(WINDOW_BUFFER_SIZE_RECORD wbsr)
{
	Borland::gotoxy(0, 13);
	printf("%s\r", blankChars);
	printf("Resize event: ");
	printf("Console screen buffer is %d columns by %d rows.\n", wbsr.dwSize.X, wbsr.dwSize.Y);
	Borland::gotoxy(0, 0);
}