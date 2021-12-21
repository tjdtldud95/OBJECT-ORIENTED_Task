#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "Position.h"
#include "Input.h"
#include "Scene.h"


using namespace std;


int main()
{
	auto screen = Screen::GetInstance();
	auto input = Input::GetInstance();
	auto scene = new Scene;

	scene->start();

	while (scene->isSceneOver() == false) 
	{
		screen->clear();

		input->readInputs();

		scene->update();

		screen->render();

		Sleep(100);
	}

	delete scene;

	return 0;
}
