#pragma once
#include "Blit3D.h"
#include "BlitElement.h"

class BlitButton : public BlitElement
{
private:
	Sprite *btnSprite[3] = { NULL };

	bool animated = true;

	void(*Input)();

	void SetTextScaleOffset(float scale_x, float scale_y);

protected:
	void Animation();

public:
	enum BtnSprite { SIMPLE = 0, HOVER, PRESSED };

	BlitButton(std::string name, float btnPos_x, float btnPos_y, float btnWidth, float btnHeight, void(*input)());

	//Functionality
	void Update(Vector2Int cursorPos);
	void Draw();
	void BtnCollision(Vector2Int cursorPos);
	void Functionality();

	//Setters
	void SetBtnSprites(Sprite *simpleBtnSprite, Sprite *hoverBtnSprite, Sprite *pressedBtnSprite);
	void SetInput(void(*func)());
	void ShowAnimation(bool show);
};