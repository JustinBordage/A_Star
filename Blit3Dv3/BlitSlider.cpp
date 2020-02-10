#include "BlitSlider.h"
#include "Blit3D.h"
#include <math.h>

#include <cassert>

extern Blit3D *blit3D;

extern bool LMB;

BlitSlider::BlitSlider(std::string name, float sliderPos_x, float sliderPos_y, float width, float height, void(*input)(float))
{
	AngelcodeFont *font = defaultTextFont[TextSize::SMALL];

	SetFont(font);
	
	//Display Name Setting
	SetSliderName(name);
	
	SetSliderTextOffset(glm::vec2(width / 2, font->FontHeight() + height / 2), glm::vec2(15.f, font->FontHeight() / 3));
	
	SetTextPos(sliderPos_x, sliderPos_y);
	
	SetSliderSprites(defaultSliderSprites[BlitSlider::GUIDE], defaultSliderSprites[BlitSlider::BUTTON]);
	
	SetBounds(sliderPos_x, sliderPos_y, width, height);
	
	SetInput(input);
	
	moveSlider(sliderPos_x);
	
	SetSliderBounds(1, 100);

	elementType = ElementType::SLIDER;
}

/*float roundToUnit(float floatToRound, float unit, float offset)
{
	floatToRound += offset;

	float numberOfUnits = floatToRound / unit;
	
	float roundedFloat = roundf(numberOfUnits) * unit;

	roundedFloat -= offset;

	return roundedFloat;
}*/

void BlitSlider::UnitConverter()
{
	//Changes the sliderVal based on the position of the slider
	float sliderVal = (sliderPos - BE_Edge.x);

	//Converts the sliderVal into a percentage
	sliderValue = sliderVal / ((BE_Edge.y - BE_Edge.x) / numOfUnits);

	//Bounds check
	if (sliderValue < LowerBounds) sliderValue = LowerBounds;
	if (sliderValue > UpperBounds) sliderValue = UpperBounds;

	int sliderTextVal = (int) sliderValue;

	//Converts the slider percentage into text
	sliderText = std::to_string(sliderTextVal);// +"%";
}

void BlitSlider::SliderCollision(Vector2Int cursorPos)
{	
	if (BE_State != SELECTED)
	{
		Collision(cursorPos);
	}
	else if (!LMB)
	{
		BE_State = NOT_IN_FOCUS;
	}
}

void BlitSlider::offsetSlider(float offset)
{
	sliderPos += offset;
}

void BlitSlider::moveSlider(float mousePos_x)
{
	sliderPos = mousePos_x;

	//Bounds check
	if (sliderPos < BE_Edge.x) sliderPos = BE_Edge.x;
	else if (sliderPos > BE_Edge.y) sliderPos = BE_Edge.y;
}

void BlitSlider::Update(Vector2Int cursorPos)
{
	SliderCollision(cursorPos);

	if (BE_State == BlitStatus::SELECTED)
	{
		moveSlider(cursorPos.x);

		UnitConverter();

		Input(sliderValue);
	}
}

void BlitSlider::Draw()
{
	//assert(btnSprite[buttonStatus] != NULL && "No Sprite is assigned to the button");
	//assert(btnFont != NULL && "No text font is assigned to the button");

	DrawBE(sliderGuide);

	//sliderGuide->Blit(sliderGuidePos.x, sliderGuidePos.y);
	sliderBtn->Blit(sliderPos, BE_Pos.y);

	//sliderFont->BlitText(sliderEdge.x + nameOffset.x, sliderEdge.w + nameOffset.y, sliderName);
	BE_Font->BlitText(BE_Edge.y + valueOffset.x, BE_Pos.y + valueOffset.y, sliderText);
}

void BlitSlider::SetSliderValue(float percent)
{
	float width = (BE_Edge.y - BE_Edge.x);
	sliderPos = BE_Edge.x + width / 100 * percent;
	UnitConverter();
}

void BlitSlider::SetSliderName(std::string displayName)
{
	BE_Text = displayName;
}

void BlitSlider::SetSliderTextOffset(glm::vec2 sliderNameOffset, glm::vec2 sliderValueOffset)
{
	BE_txtOffset = glm::vec2(-sliderNameOffset.x, sliderNameOffset.y);
	valueOffset = glm::vec2(sliderValueOffset.x, sliderValueOffset.y);
}

void BlitSlider::SetSliderSprites(Sprite *sliderGuideSprite, Sprite *sliderBtnSprite)
{
	sliderGuide = sliderGuideSprite;
	sliderBtn = sliderBtnSprite;
}

void BlitSlider::SetInput(void(*func)(float))
{
	Input = func;
}

AngelcodeFont* BlitSlider::GetSliderFont()
{
	return BE_Font;
}

void BlitSlider::SetSliderBounds(int lower, int upper)
{
	assert(lower < upper && "Lower Bounds must be smaller than Upper Bounds");

	//Sets the bounds of the slider
	LowerBounds = lower;
	UpperBounds = upper;

	//Makes the lower bounds inclusive
	//when calculating the number of units
	lower -= 1;

	//Calculates the number of units in the slider
	setUnitLength(upper - lower);
}

void BlitSlider::setUnitLength(int NumberOfUnits)
{
	numOfUnits = NumberOfUnits;
	UnitConverter();
}