#pragma once
#include <Blit3D.h>
#include "BlitElement.h"

class BlitSlider : public BlitElement
{
private:
	enum sliderState { NOT_IN_FOCUS, HOVER, SELECTED };
	
	Sprite *sliderGuide = NULL;
	Sprite *sliderBtn = NULL;

	std::string sliderText;
	
	glm::vec2 valueOffset;

	float sliderPos;
	float sliderValue;

	void(*Input)(float);

	int numOfUnits = 100;
	int UpperBounds = 100;
	int LowerBounds = 1;

	void setUnitLength(int NumberOfUnits);

public:
	enum Slider { GUIDE = 0, BUTTON };

	BlitSlider(std::string name, float sliderPos_x, float sliderPos_y, float sliderWidth, float sliderHeight, void(*input)(float));

	void UnitConverter();
	void offsetSlider(float offset);
	void moveSlider(float mousePos_x);
	void SliderCollision(Vector2Int cursorPos);
	void Update(Vector2Int cursorPos);
	void Draw();

	void SetSliderValue(float percent);
	void SetSliderName(std::string sliderName);
	void SetSliderTextOffset(glm::vec2 nameOffset, glm::vec2 valueOffset);
	void SetSliderSprites(Sprite *sliderGuide, Sprite *sliderBtn);
	void SetInput(void(*func)(float));
	void SetSliderBounds(int lower, int upper);


	AngelcodeFont* GetSliderFont();
};