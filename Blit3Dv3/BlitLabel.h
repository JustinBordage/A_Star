#pragma once
#include "BlitElement.h"
#include <Blit3D.h>

class BlitLabel : public BlitElement
{
private:

protected:
public:
	BlitLabel(AngelcodeFont* font, std::string text, float _lblPosX, float _lblPosY);
	~BlitLabel();

	void SetText(std::string text);
	void SetTextPosition(float posX, float posY);
	void SetFont(AngelcodeFont* font);
	void SetScale(float scale);

	void Draw();
};