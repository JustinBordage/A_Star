#include "BlitButton.h"
#include "Blit3D.h"

#include <cassert>

//Modified
//Did add parts to BlitMenu for animation but not complete yet

extern Blit3D *blit3D;

class BlitUI;

extern bool LMB;

BlitButton::BlitButton(std::string name, float btnPos_x, float btnPos_y, float btnWidth, float btnHeight, void(*input)())
{
	AngelcodeFont *font = defaultTextFont[TextSize::MEDIUM];

	SetFont(font);

	SetTextOffset(font->WidthText(name) / 2, font->FontHeight() / 2);

	SetTextPos(btnPos_x, btnPos_y);

	//Display Name Setting
	SetText(name);

	SetBtnSprites(defaultBtnSprites[BlitButton::SIMPLE], defaultBtnSprites[BlitButton::HOVER], defaultBtnSprites[BlitButton::PRESSED]);

	SetBounds(btnPos_x, btnPos_y, btnWidth, btnHeight);

	SetInput(input);

	elementType = ElementType::BUTTON;
}

void BlitButton::BtnCollision(Vector2Int cursorPos)
{
	Collision(cursorPos);
}

void BlitButton::Animation()
{
	//Checks if the button is animated
	if (animated)
	{
		//Checks if the button is in focus or not
		if (BE_State == BlitStatus::OUT_OF_FOCUS)
		{
			if (BE_Scale > 1.0f)
			{
				BE_Scale -= 0.01f;
				SetTextScaleOffset(BE_Scale, 1.f);
			}
		}
		else
		{
			if (BE_Scale < 1.07f)
			{
				BE_Scale += 0.01f;
				SetTextScaleOffset(BE_Scale, 1.f);
			}
		}
	}
}

void BlitButton::Functionality()
{
	//Calls the button functionality upon button release
	if (BE_State != BlitStatus::SELECTED && prev_BE_State == BlitStatus::SELECTED)
	{
		if (Input != NULL)
		{
			Input();
		}
	}
}

void BlitButton::Update(Vector2Int cursorPos)
{
	BtnCollision(cursorPos);

	Animation();

	Functionality();
}

void BlitButton::Draw()
{
	assert(BE_State < 3);//&& "This blit element state does not exist");

	DrawBE(btnSprite[BE_State]);
}

void BlitButton::SetTextScaleOffset(float scale_x, float scale_y)
{
	//Redesign to account for custom offsets

	float offset_x = BE_Font->WidthText(BE_Text) / 2 * scale_x;
	float offset_y = BE_Font->FontHeight() / 2 * scale_y;

	SetTextOffset(offset_x, offset_y);
}

void BlitButton::SetBtnSprites(Sprite *simpleBtnSprite, Sprite *hoverBtnSprite, Sprite *pressedBtnSprite)
{
	btnSprite[BlitButton::SIMPLE] = simpleBtnSprite;
	btnSprite[BlitButton::HOVER] = hoverBtnSprite;
	btnSprite[BlitButton::PRESSED] = pressedBtnSprite;
}

void BlitButton::SetInput(void(*func)())
{
	Input = func;
}

void BlitButton::ShowAnimation(bool show)
{
	animated = show;
}