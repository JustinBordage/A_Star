#pragma once
#include "BlitUI.h"
#include "LinkedList.h"

class BlitMenu : public BlitUI
{
private:
	//Datamembers
	std::string menuName;
	AngelcodeFont *menuTitleFont = NULL;
	AngelcodeFont *menuTextFont = NULL;
	Sprite *menuBacksplash = NULL;
	glm::vec2 menuBounds;
	glm::vec2 titlePos;
	void(*MenuFunction)(AngelcodeFont *textFont, glm::vec2 menuPosition) = NULL;
	bool animated = false;

	glm::vec2 menuScale;

	//Don't need to delete this since it points to
	//a list that is handled by the menu manager
	LinkedList<Sprite*>* VFXSprites;
	short VFX_Index = 0;

	void animation();
	void DrawVFX();
	void UpdateVFX(float seconds);

public:
	BlitMenu(std::string menuTitleName, Sprite *menuSprite, glm::vec2 menuPosition, glm::vec2 menuBoundaries);
	~BlitMenu();
	
	void SetMenuVFX(LinkedList<Sprite*>* spriteList);
	void SetMenuFunction(void(*func)(AngelcodeFont *textFont, glm::vec2 menuPosition));
	void SetTitleFont(AngelcodeFont *titleFont);
	void SetTextFont(AngelcodeFont *textFont);
	void SetTitleOffset(glm::vec2 title_Offset);
	void AnimateMenu(bool animated);

	void UpdateMenu(Vector2Int cursorPos, float seconds);
	void DrawMenu();
};