#pragma once
#include <Blit3D.h>
#include "BlitMenu.h"
#include "BlitUI.h"
#include "LinkedList.h"
#include "Vector2Int.h"

extern AngelcodeFont *defaultTextFont[3];
extern Sprite *defaultBtnSprites[3];
extern Sprite *defaultSliderSprites[2];

extern bool paused;

class Menu
{
private:
	Sprite* menuBacksplash[3];
	//LinkedList<Sprite*>* digitalList = NULL;

public:
	int prevMenu = 0;
	int menuStatus = PAUSEMENU;

	enum Config { NONE = -1, PAUSEMENU, SETTINGSMENU};
	enum Mode { INMENU = -1, INGAME};

	enum TextSize { SMALL = 0, MEDIUM, LARGE };
	std::vector<BlitUI *> menuList;
	std::vector<BlitUI *> UIList;

	void Init();
	void Deinit();
	void Draw();
	void Update(Vector2Int cursorPos, float seconds);
	void pause();
	void setMenuOffset(glm::vec2 offset);
	void UpdateUserInterface(BlitUI* UI, const Vector2Int& cursorPos, float seconds);
	void DrawUserInterface(BlitUI* UI);
};