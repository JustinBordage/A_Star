#include "MenuManager.h"
#include "MenuFunctionality.h"
#include <Blit3D.h>

#include <cassert>

extern Blit3D *blit3D;
extern AngelcodeFont *afont;
extern Sprite* Menu_Backsplash;

void Menu::Init()
{
	defaultTextFont[TextSize::SMALL] = blit3D->MakeAngelcodeFontFromBinary32("Media\\AsteroidText20.bin");
	defaultTextFont[TextSize::MEDIUM] = blit3D->MakeAngelcodeFontFromBinary32("Media\\AsteroidText45.bin");
	defaultTextFont[TextSize::LARGE] = blit3D->MakeAngelcodeFontFromBinary32("Media\\AsteroidText100.bin");

	for (int btn = 0; btn < 3; btn++)
	{
		defaultBtnSprites[btn] = blit3D->MakeSprite(5, 5 + btn * 130, 450, 120, "Media\\defaultBtnSprites.png");
	}

	defaultSliderSprites[BlitSlider::GUIDE] = blit3D->MakeSprite(0, 480, 210, 20, "Media\\GameAssets.png");
	defaultSliderSprites[BlitSlider::BUTTON] = blit3D->MakeSprite(215, 480, 20, 20, "Media\\GameAssets.png");

	//digitalList = new LinkedList<Sprite*>();

	menuBacksplash[SMALL] = blit3D->MakeSprite(0, 0, 710, 480, "Media\\PauseMenu_Backsplash.png");
	menuBacksplash[MEDIUM] = blit3D->MakeSprite(0, 480, 710, 910, "Media\\PauseMenu_Backsplash.png");

	glm::vec2 centerPos = glm::vec2(blit3D->screenWidth / 2, blit3D->screenHeight / 2);

	//Since the sprites are optimised for 1080p screens, 1920p X 1080p will become reference points
	//glm::vec2 scaling = glm::vec2(blit3D->screenWidth / 1920.f, blit3D->screenHeight / 1080.f);

	BlitSlider* Slider;
	BlitButton* Button;
	//BlitLabel* Label;

	//---------------------------------------------------------------------------------------------------------------------------------------------------
	//															IN-GAME UI
	//---------------------------------------------------------------------------------------------------------------------------------------------------

	glm::vec2 menuPos = glm::vec2(centerPos.x, centerPos.y - 420.f);
	BlitMenu *UI_Controls = new BlitMenu("", Menu_Backsplash, menuPos, glm::vec2(1920.f, 240.f));

	Slider = UI_Controls->newSlider("Terrain Heuristic", -140.f, 20.f, 200.f, 20.f, HeuristicSlider);
	Slider->SetSliderBounds(1, 15);
	Slider->SetSliderValue(1);

	UI_Controls->newLabel(afont, "Tiles:  ", -930.f, -20.f);
	UI_Controls->newLabel(afont, "X: ", -930.f, 80.f);
	UI_Controls->newLabel(afont, "Y: ", -710.f, 80.f);

	Button = UI_Controls->newButton("Load Map 1", 425.f, 30.f, 225.f, 60.f, LoadMapOne);
	Button->SetScale(0.5f);
	Button->SetTextOffset(70.f, 10.f);
	Button->ShowAnimation(false);

	Button = UI_Controls->newButton("Load Map 2", 425.f, -40.f, 225.f, 60.f, LoadMapTwo);
	Button->SetScale(0.5f);
	Button->SetTextOffset(70.f, 10.f);
	Button->ShowAnimation(false);

	Button = UI_Controls->newButton("Load Map 3", 675.f, 30.f, 225.f, 60.f, LoadMapThree);
	Button->SetScale(0.5f);
	Button->SetTextOffset(70.f, 10.f);
	Button->ShowAnimation(false);

	Button = UI_Controls->newButton("Load Custom Map", 675.f, -40.f, 225.f, 60.f, LoadCustomMap);
	Button->SetScale(0.5f);
	Button->SetTextOffset(100.f, 10.f);
	Button->ShowAnimation(false);

	Button = UI_Controls->newButton("Save Custom Map", 0.f, -50.f, 225.f, 60.f, SaveCustomMap);
	Button->SetScale(0.5f);
	Button->SetTextOffset(100.f, 10.f);
	Button->ShowAnimation(false);

	Button = UI_Controls->newButton("Find Path", -280.f, -50.f, 225.f, 60.f, FindRoverPath);
	Button->SetScale(0.5f);
	Button->SetTextOffset(60.f, 10.f);
	Button->ShowAnimation(false);

	UIList.push_back(UI_Controls);

	//---------------------------------------------------------------------------------------------------------------------------------------------------
	//															PAUSE MENU
	//---------------------------------------------------------------------------------------------------------------------------------------------------

	BlitMenu *pauseMenu = new BlitMenu("Paused", menuBacksplash[MEDIUM], centerPos, glm::vec2(710.f, 910.f));
	pauseMenu->newButton("Resume", 0.f, -15.f, 450.f, 120.f, resume);
	//pauseMenu->newButton("Settings", 0.f, -145.f, 450.f, 120.f, settings);
	pauseMenu->newButton("Exit", 0.f, -205.f, 450.f, 120.f, exit);

	menuList.push_back(pauseMenu);

	//---------------------------------------------------------------------------------------------------------------------------------------------------
	//														  SETTINGS MENU
	//---------------------------------------------------------------------------------------------------------------------------------------------------

	BlitMenu *settingsMenu = new BlitMenu("Settings", menuBacksplash[MEDIUM], centerPos, glm::vec2(710.f, 910.f));
	settingsMenu->newSlider("Sensitivity", -170.f, 145.f, 200.f, 20.f, sensitivitySlider);
	settingsMenu->newButton("Back", 0.f, -325.f, 450.f, 120.f, back);

	menuList.push_back(settingsMenu);
}

void Menu::Deinit()
{
	for (int listNum = 0; listNum < menuList.size(); listNum++)
	{
		menuList[listNum]->DeinitUI();
		delete menuList[listNum];
	}
	menuList.clear();

	for (int listNum = 0; listNum < UIList.size(); listNum++)
	{
		UIList[listNum]->DeinitUI();
		delete UIList[listNum];
	}
	UIList.clear();

	//delete digitalList;
}

void Menu::setMenuOffset(glm::vec2 offset)
{
	offset = -offset;

	for (BlitUI* menu : menuList)
	{
		menu->SetOffset(offset);
	}

	for (BlitUI* UI : UIList)
	{
		UI->SetOffset(offset);
	}
}

void Menu::UpdateUserInterface(BlitUI* UI, const Vector2Int& cursorPos, float seconds)
{
	if (UI->type == UI_Type::MENU)
	{
		((BlitMenu*)UI)->UpdateMenu(cursorPos, seconds);
	}
	else
		UI->UpdateUI(cursorPos);
}

void Menu::DrawUserInterface(BlitUI* UI)
{
	if (UI->type == UI_Type::MENU)
	{
		((BlitMenu*)UI)->DrawMenu();
	}
	else
		UI->DrawUI();
}

void Menu::Update(Vector2Int cursorPos, float seconds)
{
	BlitUI* currentUI = NULL;

	if (paused)
	{
		if (menuList.size() > 0)
		{
			currentUI = menuList[menuStatus];
		}
	}
	else
	{
		if (UIList.size() > 0)
		{
			currentUI = UIList[Mode::INGAME];
		}
	}

	assert(currentUI != NULL && "This UI does not exist");

	UpdateUserInterface(currentUI, cursorPos, seconds);
}

void Menu::Draw()
{
	BlitUI* currentUI = NULL;

	if (paused)
	{
		if (menuList.size() > 0)
		{
			currentUI = menuList[menuStatus];
		}
	}
	else
	{
		if (UIList.size() > 0)
		{
			currentUI = UIList[Mode::INGAME];
		}
	}

	assert(currentUI != NULL && "This UI object does not exist");

	DrawUserInterface(currentUI);
}

void Menu::pause()
{
	paused = !paused;
	
	if (paused)
	{
		menuStatus = Menu::Config::PAUSEMENU;
	}
	else
	{
		menuStatus = Menu::Config::NONE;
	}
}