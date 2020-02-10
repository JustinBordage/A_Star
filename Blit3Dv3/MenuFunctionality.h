#pragma once
#include <Blit3D.h>



//Menu Functionality
void scoreMenuText(AngelcodeFont *textFont, glm::vec2 menuPosition);
void gameOverMenuText(AngelcodeFont *textFont, glm::vec2 menuPosition);

//Button Functionality

	//---General---
	void resume();
	void back();
	void exit();

	//---Unique---
	void start();
	void settings();

	void LoadMapOne();
	void LoadMapTwo();
	void LoadMapThree();
	void LoadCustomMap();
	void SaveCustomMap();
	void FindRoverPath();


//Slider Functionality
void sensitivitySlider(float percent);
void HeuristicSlider(float percent);