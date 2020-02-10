#include "BlitLabel.h"
#include <iostream>

BlitLabel::BlitLabel(AngelcodeFont* _font, std::string _text, float _lblPosX, float _lblPosY)
{
	this->SetFont(_font);
	this->SetTextPosition(_lblPosX, _lblPosY);
	this->SetText(_text);
	this->SetScale(1.f);

	this->elementType = ElementType::LABEL;
}

BlitLabel::~BlitLabel()
{

}

void BlitLabel::SetText(std::string _text)
{
	this->BE_Text = _text;
}

void BlitLabel::SetTextPosition(float _posX, float _posY)
{
	this->BE_Pos = glm::vec2(_posX, _posY);
}

void BlitLabel::SetFont(AngelcodeFont* _font)
{
	this->BE_Font = _font;
}

void BlitLabel::SetScale(float _scale)
{
	this->BE_Scale = _scale;
}

void BlitLabel::Draw()
{
	assert(BE_Font != NULL && "No text font is assigned to the button");

	BE_Font->BlitText(BE_Pos.x, BE_Pos.y, BE_Scale, BE_Scale, BE_Text);
}