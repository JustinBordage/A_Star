#include "BlitElement.h"

extern bool LMB;

void BlitElement::Collision(Vector2Int cursorPos)
{
	prev_BE_State = BE_State;

	if (cursorPos.x > BE_Edge.x && cursorPos.x < BE_Edge.y)
	{
		if (cursorPos.y > BE_Edge.z && cursorPos.y < BE_Edge.w)
		{
			if (LMB)
			{
				BE_State = BlitStatus::SELECTED;
			}
			else
			{
				BE_State = BlitStatus::IN_FOCUS;
			}
		}
		else BE_State = BlitStatus::OUT_OF_FOCUS;
	}
	else BE_State = BlitStatus::OUT_OF_FOCUS;
}

void BlitElement::UpdateBE(Vector2Int cursorPos)
{
	Collision(cursorPos);
}

void BlitElement::DrawBE(Sprite *BlitSprite)
{
	assert(BlitSprite != NULL && "No Sprite is assigned to the button");
	assert(BE_Font != NULL && "No text font is assigned to the button");

	BlitSprite->Blit(BE_Pos.x, BE_Pos.y, BE_Scale, BE_Scale);
	BE_Font->BlitText(BE_txtPos.x + BE_txtOffset.x, BE_txtPos.y + BE_txtOffset.y, BE_Scale, BE_Scale, BE_Text);
}

void BlitElement::SetText(std::string text)
{
	BE_Text = text;
}

void BlitElement::SetTextPos(float pos_x, float pos_y)
{
	BE_txtPos.x = pos_x;
	BE_txtPos.y = pos_y;
}

void BlitElement::SetTextOffset(float offset_x, float offset_y)
{
	BE_txtOffset.x = -offset_x;
	BE_txtOffset.y = offset_y;
}

void BlitElement::SetPosition(float pos_x, float pos_y)
{
	//Position setting
	BE_Pos = glm::vec2(pos_x, pos_y);

	//Edge Setting		(Left, Right, Bottom, Top)
	BE_Edge = glm::vec4(pos_x - halfSize.x, pos_x + halfSize.x,
						pos_y - halfSize.y, pos_y + halfSize.y);

	SetTextPos(pos_x, pos_y);
}

void BlitElement::SetBounds(float pos_x, float pos_y, float width, float height)
{
	//Edge Offset
	halfSize = glm::vec2(width / 2, height / 2);

	//Position setting
	SetPosition(pos_x, pos_y);
}

void BlitElement::SetFont(AngelcodeFont *txtFont)
{
	BE_Font = txtFont;
}

void BlitElement::SetScale(float scale)
{
	BE_Scale = scale;
}

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