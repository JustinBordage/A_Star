#pragma once
#include "BlitButton.h"
#include "BlitSlider.h"
#include "BlitLabel.h"
#include "LinkedList.h"

enum class UI_Type { UI, MENU };

class BlitUI
{
private:

protected:
	glm::vec2 UI_Pos;
	glm::vec2 UI_Offset;

public:
	std::vector<BlitElement *> elementList;

	UI_Type type = UI_Type::UI;

	enum TextSize { SMALL = 0, MEDIUM, LARGE };

	BlitUI(float userInterfacePos_X, float userInterfacePos_Y);


	//Debug Button maker
	//void newButton(std::string displayName, glm::vec2 btnPos, glm::vec2 btnBoundries, void(*input)(void));
	
	BlitButton* newButton(std::string displayName, float positionOffset_x, float positionOffset_y, float btnWidth, float btnHeight, void(*input)());
	BlitSlider* newSlider(std::string displayName, float posOffset_x, float posOffset_y, float sliderWidth, float sliderHeight, void(*input)(float));
	BlitLabel* newLabel(AngelcodeFont* font, std::string text, float posOffset_x, float posOffset_y);
	void DeinitUI();

	void UpdateUI(Vector2Int cursorPos);
	void DrawUI();
	void SetButtonFont(AngelcodeFont *btnTxtFont);
	void SetButtonSprites(Sprite *simpleBtn, Sprite *hoverBtn, Sprite *pressedBtn);
	void AnimateButtons(bool animated);

	void SetSliderFont(AngelcodeFont *sliderTxtFont);
	void SetSliderSprites(Sprite *sliderGuide, Sprite *sliderBtn);
	void SetSliderTextOffset(float nameOffset_x, float nameOffset_y, float valueOffset_x, float valueOffset_y);

	void SetOffset(glm::vec2 Offset);
};