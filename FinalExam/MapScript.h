#pragma once
#include "GameObject.h"
#include "Behaviour.h"
#include <iostream>
#include <fstream>
class MapScript :
	public Behaviour
{
	bool* map;
	static const char rectFill = '\xdb'; //

public :
	MapScript(GameObject* gameObject) : Behaviour(gameObject),
		map(nullptr)
	{
		auto dim = renderer->getDimension();
		map = new bool[(size_t)dim.x * dim.y];
		for (int i = 0;i < dim.x * dim.y;i++) map[i] = false;

		load((size_t)dim.x * dim.y);
	}
	~MapScript() { if (map) delete[] map; }

	void load(int capacity)
	{
		ifstream mapfile;
		char* data = new char[capacity];
		
		mapfile.open("map.txt");
		memset(map, false, sizeof(bool) * capacity);

		if (!mapfile.is_open()) return;

		if (!mapfile.eof()) {
			mapfile.getline(data, capacity,'.');
		}

		mapfile.close();
		for (int i = 0; i < capacity; i++)
		{
			if (data[i] == '1' ) map[i] = true;
		}
		delete[] data;
	}

	void update()
	{
		auto dim = renderer->getDimension();
		auto height = dim.y;
		auto width = dim.x;
		for (auto i =0; i < height; i++) 
		{
			for (auto j = 0; j < width; j++)
			{			
				renderer->setShape(map[j + i * width] ? rectFill : ' ', j + i * width);
			}
		}
	}

	void clear() {
		auto dim = renderer->getDimension();
		auto size = size_t(dim.x * dim.y);
		
		memset(map, false, size);
		load(size);
	}

	bool isValidRange(const Position& nextPos, const Dimension& sz)
	{
		auto pos = transform->getPos();
		if (nextPos.x < 0 || nextPos.x > renderer->getWidth() || nextPos.y < 0 || nextPos.y > renderer->getHeight()) return false;
		if (nextPos.x + sz.x > renderer->getWidth() || nextPos.y + sz.y > renderer->getHeight()) return false;
		return true;
	}

	void place(const char* shape, const Position& pos, int w, int h) 
	{
		auto dim = renderer->getDimension();
		for (int i = 0; i < h; i++) {
			for (int j = 0; j < w; j++) {
				if (shape[j + i * w] != ' ') {
					map[pos.x-1+ j + (pos.y + i) * dim.x] = true;
					renderer->setShape(rectFill, { pos.x + j, pos.y + i });
				}
			}
		}
	}

	bool isCruched(const char* shape , const Position& pos,int w,int h)
	{
		auto dim = renderer->getDimension();
		auto height = dim.y;
		auto width = dim.x;

		if (pos.y < 1 || pos.y >=19) return true;

		auto capacity = renderer->getCapacity();

		for (auto i = 0;i < w;i++)
		{
			for (auto j = 0;j < h;j++)
			{
				if (shape[j + i * w] != ' ')
				{
					auto nextOffset = pos.x + i + (pos.y + j) * width;
					if (nextOffset >= capacity) return true;
					if (map[nextOffset] == true) return true;
				}
			}
		}
	
		return false;
	}



};

