#pragma once
#include "Blit3D.h"
#include "Vector2Int.h"
//#include "BlitUI.h"

extern AngelcodeFont *defaultTextFont[3];
extern Sprite *defaultBtnSprites[3];
extern Sprite *defaultSliderSprites[2];

enum class ElementType { LABEL, BUTTON, SLIDER };

class BlitElement
{
protected:
	//BlitUI* parent = NULL;

	AngelcodeFont * BE_Font = NULL;
	float txtWidth = 0.f;

	enum BlitStatus { OUT_OF_FOCUS, IN_FOCUS, SELECTED };
	
	std::string BE_Text;
	glm::vec2 BE_txtOffset = glm::vec2(0.f, 0.f);
	glm::vec2 BE_txtPos;

	int BE_State = BlitStatus::OUT_OF_FOCUS;
	int prev_BE_State = BlitStatus::OUT_OF_FOCUS;

	float BE_Scale = 1.f;
	glm::vec4 BE_Edge;
	glm::vec2 halfSize;

	void Collision(Vector2Int cursorPos);
	void UpdateBE(Vector2Int cursorPos);
	void DrawBE(Sprite *BlitSprite);

public:
	enum TextSize { SMALL = 0, MEDIUM, LARGE };

	ElementType elementType = ElementType::LABEL;
	glm::vec2 BE_Pos;

	void SetText(std::string text);
	void SetTextPos(float pos_x, float pos_y);
	void SetTextOffset(float offset_x, float offset_y);
	void SetBounds(float pos_x, float pos_y, float width, float height);
	void SetPosition(float pos_x, float pos_y);
	void SetFont(AngelcodeFont *txtFont);
	void SetScale(float scale);
};

/*
switch (blitState)
{
case OUT_OF_FOCUS:
{

break;
}
case IN_FOCUS:
{

break;
}
case SELECTED:
{

break;
}
default:
{
assert(blitState < 3 && "This blit element state does not exist");
break;
}
}
*/


//----------BlitSlider----------
/*


Sprite *sliderGuide = NULL;
Sprite *sliderBtn = NULL;

std::string sliderText;

glm::vec2 valueOffset;

float sliderPos;
float sliderValue;
*/