#include "BlitMenu.h"
#include "MenuManager.h"

BlitMenu::BlitMenu(std::string menuTitleName, Sprite *menuSprite, glm::vec2 menuPosition, glm::vec2 menuBoundaries) : BlitUI(menuPosition.x, menuPosition.y)
{
	//Sets the default fonts
	menuTitleFont = defaultTextFont[LARGE];
	menuTextFont = defaultTextFont[MEDIUM];

	//Sets the menu values
	menuName = menuTitleName;
	menuBacksplash = menuSprite;
	menuBounds = menuBoundaries;

	//Sets the default title offset (Centered, 165 pixels below top edge)
	titlePos.y = UI_Pos.y + (menuBounds.y / 2.f) - 165.f;
	titlePos.x = UI_Pos.x;

	VFXSprites = NULL;
	menuScale = glm::vec2(1.f, 1.f);

	type = UI_Type::MENU;
}

BlitMenu::~BlitMenu()
{
	DeinitUI();
}

void BlitMenu::DrawVFX()
{
	if (VFXSprites != NULL)
	{
		if (VFXSprites->size() > 0)
		{
			Sprite* tempSprite = VFXSprites->get(VFX_Index);

			tempSprite->Blit(UI_Pos.x, UI_Pos.y);
		}
	}
}

void BlitMenu::UpdateVFX(float seconds)
{
	if (VFXSprites != NULL)
	{
		short size = VFXSprites->size();

		if (size > 0)
		{
			static float elapsedTime = 0;

			elapsedTime += seconds;

			if (elapsedTime > 0.02f)
			{
				VFX_Index++;

				if (VFX_Index >= size)
					VFX_Index -= size;

				elapsedTime -= 0.02f;
			}
		}
	}
}

void BlitMenu::animation()
{
	//Checks if the button is animated
	if (animated)
	{
		//Checks if the button is in focus or not
		if (!paused)
		{
			if (menuScale.x > 1.0f)
			{
				menuScale -= 0.01f;
			}
		}
		else
		{
			if (menuScale.x < 1.07f)
			{
				menuScale += 0.01f;
			}
		}
	}
}

void BlitMenu::UpdateMenu(Vector2Int cursorPos, float seconds)
{
	//Updates the VFX animation frame
	UpdateVFX(seconds);

	//Updates the functionality of all the UI Elements
	UpdateUI(cursorPos);
}

void BlitMenu::DrawMenu()
{
	//Displays the backsplash of the menu
	menuBacksplash->Blit(UI_Pos.x + UI_Offset.x, UI_Pos.y + UI_Offset.y);

	//Displays the title of the menu
	menuTitleFont->BlitAutoText(titlePos.x + UI_Offset.x, titlePos.y + UI_Offset.y, menuName);

	//Displays all of the UI Elements
	DrawUI();

	//Calls the functionality of the menu
	if (MenuFunction != NULL)
	{
		MenuFunction(menuTextFont, UI_Pos + UI_Offset);
	}

	//Draws the current VFX animation frame
	DrawVFX();
}

void BlitMenu::SetMenuVFX(LinkedList<Sprite*>* _spriteList)
{
	VFXSprites = _spriteList;
}

void BlitMenu::SetMenuFunction(void(*func)(AngelcodeFont *textFont, glm::vec2 menuPosition))
{
	MenuFunction = func;
}

void BlitMenu::SetTitleFont(AngelcodeFont *titleFont)
{
	menuTitleFont = titleFont;
}

void BlitMenu::SetTextFont(AngelcodeFont *textFont)
{
	menuTextFont = textFont;
}

void BlitMenu::SetTitleOffset(glm::vec2 title_Offset)
{
	//Sets the title offset
	titlePos.y = (menuBounds.y / 2.f) - title_Offset.y; //Offsets from top to bottom (smallest to largest)
	titlePos.x = title_Offset.x - (menuBounds.x / 2.f);	//Offsets from left to right (smallest to largest)
}

void BlitMenu::AnimateMenu(bool menuAnimated)
{
	animated = menuAnimated;
}
