#define _CRT_SECURE_NO_WARNINGS

#include<iostream>
#include<Windows.h>
#include<conio.h>
#include<string>
#include"Utils.h"

class Screen;
class Block;
class BlockManager;
class Input;
class GameManager;


class Screen {
private:
	int		width; // visible width
	int		height;
	int     size;
	char** canvas;

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

		size = (this->width + 1) * this->height;

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
	static Screen* instance;

	static Screen* getInstance()
	{
		if (instance == nullptr)
			instance = new Screen(10, 20);

		return instance;
	}

	int getSize()
	{
		return size;
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
			memset(canvas[i], ' ', width + 1);
			canvas[i][0] = 5;
			canvas[i][width + 1] = 5;
			canvas[i][width + 2] = '\0';
			if (i == 0 || i == height - 1)
			{
				memset(canvas[i], 6, width + 1);
			}
		}

		canvas[0][0] = 1;
		canvas[0][width + 1] = 2;
		canvas[height - 1][0] = 3;
		canvas[height - 1][width + 1] = 4;
	}

	void render()
	{
		Borland::gotoxy(0, 0);
		for (int i = 0;i < height;i++)
		{
			printf("%s\n", canvas[i]);
		}
	}

	void draw(const Position& pos, const char faceblocknum)
	{
		canvas[pos.y][pos.x] = faceblocknum;
	}

};
Screen* Screen::instance = nullptr;
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
Input* Input::Instance = nullptr;
class GameManager
{
private:
	Screen* screen;
	char** map;

	GameManager() : screen(Screen::getInstance())
	{
		map = (char**)malloc(sizeof(char*) * screen->getHeigt()-2);
		for (int i = 0;i < screen->getHeigt();i++)
		{
			map[i] = (char*)malloc(sizeof(char) * screen->getWidth());
			memset(map[i], 0, screen->getWidth()-1);
		}
	
	}

	~GameManager()
	{
		for (int i = 0;i < screen->getHeigt();i++)
		{
			free(map[i]);
		}
	}

public:
	static GameManager* instance;

	static GameManager* getInstance()
	{
		if (instance == nullptr)
			instance = new GameManager();

		return instance;
	}


	void positionMap(const Position& pos)
	{
		map[pos.y][pos.x] = 1;
	}

	bool IsEmpty(const Position pos)
	{
		if (map[pos.y][pos.x] == 1)
			return false;

		return true;
	}

	bool isFull(int num)
	{
		int size = 0;
		for (int i = 0;i < screen->getWidth() + 1;i++)
		{
			if (map[num][i] == 1)
				size++;
		}

		return size == 10;
	}

	void draw()
	{
		for (int i = 0;i < screen->getHeigt() - 1;i++)
		{
			for (int j = 0;j < screen->getWidth()+1;j++)
			{
				if (map[i][j] == 1)
				{
					screen->draw(Position(j, i), 'X');
				}
			}
		}
	}

	void update()
	{
		int i;
		bool isfull = false;
		for (i = 0;i < screen->getHeigt() - 1;i++)
		{
			isfull=isFull(i);
			if (isfull)
				break;
		}

		if (isfull) // not yet
		{
			for (int j = 1;j < screen->getWidth() + 1;j++)
			{
				map[i][j] = 2;
				screen->draw(Position(j, i), '+');
			}

			char tmp[20] = { 0 };
			Borland::gotoxy(0, 20);
			for(int j = i; j > 1;j--)
			{
				printf("%d ", j);
				strcpy(tmp, map[j]);
				strcpy(map[j], map[j - 1]);
				strcpy(map[j - 1], tmp);
			}
		}
	}

};
GameManager* GameManager::instance = nullptr;
class Block
{
private:
	Position blockpos[9];
	Position   mainPos;
	bool destroy;
	bool isLeftBlock;
	bool isRightBlock;
	char face[3][3];
	int faceblocklength;

public:
	Block()
	{
		destroy = false;
		mainPos.x = 5;
		mainPos.y = 2;
		for (int i = 0;i < 3;i++)
		{
			memset(face[i], ' ', 3);
		}
	}
	~Block()
	{
		mainPos = NULL;
	}
	
	void setBlockface(const Position& pos,char shape)
	{
		face[pos.y][pos.x] = shape;
	}

	void setBlockface(const Position arr[],int len,char shape)
	{
		faceblocklength = len;
		for (int i = 0;i < len;i++)
		{
			blockpos[i] = arr[i];
			face[arr[i].y][arr[i].x] = shape;
		}
		
	}

	void BlockMove(const char& keyCode) // R : Right   L:left   D : down
	{
		if (keyCode == 'L')
		
			this->mainPos.x--;
		
			
		else if (keyCode == 'R')
			this->mainPos.x++;
		

		else if (keyCode == 'D')
			this->mainPos.y++;

	}

	void rotateLeftBlockface()
	{
		char tmp[3][3];
		int index = 0;

		if (mainPos.x >= 9)
			mainPos.x--;
		if (mainPos.x <= 1)
			mainPos.x++;


		for (int i = 0;i < 3;i++)
		{
			for (int j = 0;j < 3;j++)
			{
				tmp[j][3 - i - 1] = this->face[i][j];
			}
		}

		for (int i = 0;i < 3;i++)
		{
			for (int j = 0;j < 3;j++)
			{
				this->face[i][j] = tmp[i][j];
			}
		}
		
		for (int i = 0;i < 3;i++)
		{
			for (int j = 0;j < 3;j++)
			{
				if (face[i][j] == 'O')
				{
					this->blockpos[index] = Position(i, j);
					index++;
				}
			}
		}

	}

	Position getMainPos()
	{
		return mainPos;
	}

	void setMainPos(const Position pos)
	{
		mainPos.x = pos.x;
		mainPos.y = pos.y;
	}

	Position getBlockPos(int num)
	{
		return blockpos[num];
	}

	char getBlockface()
	{
		for (int i = 0;i < 3;i++)
		{
			for (int j = 0;j < 3;j++)
			{
				if (face[i][j] != ' ')
				{
					return this->face[i][j];
				}
			}
		}
	}

	int getBlockPosLength()
	{
		return faceblocklength;
	}

	void setDestoy(bool value)
	{
		destroy = value;
	}

	bool getDestoy()
	{
		return destroy;
	}
};

class BlockManager
{
	enum class BlockFaces
	{
		//ㄱ, ㅁ, ㅣ, ㄹ , ㄹ반대
		Atype = 0,
		Btype,
		Ctype,
		Dtype,
		Etype
	};

	const Position Atype[5] = { Position(0,0), Position(1,0),Position(2,0), Position(2,1),Position(2,2) };
	const Position Btype[4] = { Position(0,0), Position(1,0),Position(0,1),Position(1,1) };
	const Position Ctype[3] = { Position(0,1),Position(1,1),Position(2,1) };
	const Position Dtype[4] = { Position(0,2),Position(0,1),Position(1,1),Position(1,0) };
	const Position Etype[4] = { Position(0,0),Position(0,1),Position(1,1),Position(1,2) };

	Block* blocks;
	GameManager* gameManager;
	Input* input;
	Screen* screen;
	int key;

public:
	BlockManager() : gameManager(GameManager::getInstance()), input(Input::GetInstance()), screen(Screen::getInstance())
	{
		key = 0;
		srand(time(nullptr));

		blocks = new Block[3];
		for (int i = 0;i < 3;i++)
		{
			BlockFaces type = BlockFaces(2); //rand() % 5
			switch (type)
			{
			case BlockManager::BlockFaces::Atype:
				blocks[i].setBlockface(Atype, 5, 'O');
				break;
			case BlockManager::BlockFaces::Btype:
				blocks[i].setBlockface(Btype, 4, 'O');
				break;
			case BlockManager::BlockFaces::Ctype:
				blocks[i].setBlockface(Ctype, 3, 'O');
				break;
			case BlockManager::BlockFaces::Dtype:
				blocks[i].setBlockface(Dtype, 4, 'O');
				break;
			case BlockManager::BlockFaces::Etype:
				blocks[i].setBlockface(Etype, 4, 'O');
				break;
			default:
				break;
			}
		}

	}

	~BlockManager()
	{
		free(blocks);
	}


	void resetBlocks(int num)
	{
		Block* b = new Block();
		blocks[num] = *b;

		BlockFaces type = BlockFaces(2);//rand() % 5
		switch (type)
		{
		case BlockManager::BlockFaces::Atype:
			blocks[num].setBlockface(Atype, 5, 'O');
			break;
		case BlockManager::BlockFaces::Btype:
			blocks[num].setBlockface(Btype, 4, 'O');
			break;
		case BlockManager::BlockFaces::Ctype:
			blocks[num].setBlockface(Ctype, 3, 'O');
			break;
		case BlockManager::BlockFaces::Dtype:
			blocks[num].setBlockface(Dtype, 4, 'O');
			break;
		case BlockManager::BlockFaces::Etype:
			blocks[num].setBlockface(Etype, 4, 'O');
			break;
		default:
			break;
		}

	}

	void draw()
	{
		int len = blocks[key].getBlockPosLength();

		for (int i = 0;i < len;i++)
		{
			Position drawpos = blocks[key].getMainPos();
			drawpos.x += blocks[key].getBlockPos(i).x;
			drawpos.y += blocks[key].getBlockPos(i).y;	
			screen->draw(drawpos, blocks[key].getBlockface());
		}
	}

	void update()
	{
		if (blocks[key].getDestoy())
		{
			resetBlocks(key);
			key++;
			if (key >= 3)
				key = 0;
			return;
		}
		int len = blocks[key].getBlockPosLength();
		if (input->getKey(VK_LEFT))
		{
			for (int i = 0;i < len;i++)
			{
				Position currentPos = blocks[key].getMainPos();
				Position pos = blocks[key].getBlockPos(i);
				currentPos.x += pos.x;

				if (currentPos.x <= 1)
					return;

				currentPos.x--;
				currentPos.y += pos.y;

				bool canmove = gameManager->IsEmpty(currentPos);

				if (!canmove) return;
			}

			blocks[key].BlockMove('L');

		}
		
		if (input->getKey(VK_RIGHT))
		{
			for (int i = 0;i < len;i++)
			{
				Position currentPos = blocks[key].getMainPos();
				Position pos = blocks[key].getBlockPos(i);
				currentPos.x += pos.x;

				if (currentPos.x >= 10)
					return;

				currentPos.x++;
				currentPos.y += pos.y;
				bool canmove = gameManager->IsEmpty(currentPos);

				if (!canmove) return;

			}
			blocks[key].BlockMove('R');
		}

		if (input->getKey(VK_UP))
		{
			blocks[key].rotateLeftBlockface();
		}

		if (input->getKey(VK_DOWN))
		{
			//check touch something by block move 
			bool canmove = false;
			for (int i = 0;i < len;i++)
			{
				Position pos = blocks[key].getBlockPos(i);
				Position currentPos = blocks[key].getMainPos();
				currentPos.x += pos.x;
				currentPos.y += pos.y;
				currentPos.y++;
				canmove = gameManager->IsEmpty(currentPos);

				if (!canmove) break;
				if (currentPos.y >= 19)
				{
					canmove = false;
					break;
				}
			}

			if(canmove)
				blocks[key].BlockMove('D');

			else
			{
				for (int i = 0;i < len;i++)
				{
					Position pos = blocks[key].getBlockPos(i);
					Position mainPos = blocks[key].getMainPos();
					mainPos.x += pos.x;
					mainPos.y += pos.y;

					blocks[key].setBlockface(pos, 'X');
					gameManager->positionMap(mainPos);
				}
				blocks[key].setDestoy(true);
			}
		}

		if (input->getKey(VK_SPACE))
		{
			int y = 0;
			bool canmove = false;
			while (1)
			{
				for (int i = 0;i < len;i++)
				{
					Position pos = blocks[key].getBlockPos(i);
					Position currentPos = blocks[key].getMainPos();
					currentPos.x += pos.x;
					currentPos.y += pos.y;
					currentPos.y +=y;
					canmove = gameManager->IsEmpty(currentPos);

					if (!canmove) break;
					if (currentPos.y >= 19)
					{
						canmove = false;
						break;
					}
				}
				
				if (!canmove)
					break;

				y++;
			}

			for (int i = 0;i < len;i++)
			{
				Position pos = blocks[key].getBlockPos(i);
				Position mainPos = blocks[key].getMainPos();
				mainPos.x += pos.x;
				mainPos.y += pos.y;
				mainPos.y += y-1;

				blocks[key].setBlockface(pos, 'X');
				gameManager->positionMap(mainPos);
			}

			blocks[key].setDestoy(true);
		}
		
	}
	
};


int main()
{
	Screen* screen = Screen::getInstance();
	GameManager* gm = GameManager::getInstance();
	Input* input = Input::GetInstance();
	BlockManager b;

	while (1)
	{
		screen->clear();

		gm->draw();
		b.draw();

		input->readInputs();

		b.update();

		gm->update();
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

