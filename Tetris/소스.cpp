#define _CRT_SECURE_NO_WARNINGS

#include<iostream>
#include<Windows.h>
#include<conio.h>
#include<string>
#include"Utils.h"

class Screen;
class Block;

class Screen {
private:
	int		width; // visible width
	int		height;
	char** canvas;

public:
	// constructor (생성자 함수) 메모리공간상에 적재되는 순간 호출되는
	Screen(int width=10, int height =10)
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
			canvas[i] = (char*)malloc(sizeof(char) * width+1);
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

	void draw(Block* block);
};

class Block 
{
private:
	Position pos;
	char face[3][3] = { 127 ,127,127,
						 ' ' ,' ',127,
						 ' ' ,' ',127 };
	/*
	{ 127 ,127,127,
	  ' ' ,' ',127, 
	  ' ' ,' ',127 };
	
	*/
public:
	Block()
	{
		pos.x = 5;
		pos.y = 2;
	}
	void set_Blockface(char blockface[][3])
	{
		for (int i = 0;i < 3;i++)
		{
			for (int j = 0;j < 3;j++)
			{
				face[i][j] = blockface[i][j];
			}
		}
	}

	char get_Blockface_num(int hei,int wid)
	{
		return face[hei][wid];
	}

	Position get_Pos()
	{
		return this->pos;
	}

	void rotate_Blockface()
	{
		
		char tmp[3][3];
		
		for (int i = 0;i < 3;i++)
		{
			for (int j = 0;j < 3;j++)
			{
				tmp[j][3 - i - 1] = this->face[i][j];
			}
		}
		
		this->set_Blockface(tmp);
	}
};

void Screen::draw(Block *block)
{
	Position pos = block->get_Pos();

	for (int i = -1;i <= 1;i++)
	{
		for (int j = -1;j <= 1;j++)
		{
			canvas[pos.y - i][pos.x - j] = block->get_Blockface_num(i+1,j+1);//0 1 2
		}
	}
}
int main()
{
	Screen *screen = new Screen(10,15);
	Block b;

	screen->clear();

	while (1)
	{
		screen->clear();
		screen->draw(&b);
		screen->render();

		int a = _getch();

		if (a == 'q')
		{
			b.rotate_Blockface();
		}
		
		Sleep(100);
	}

	return 0;
}