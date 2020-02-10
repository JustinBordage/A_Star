#include "BlitUI.h"
#include "BlitButton.h"
#include "Blit3D.h"
#include "MenuManager.h"
#include "BlitElement.h"

#include <cassert>

extern Blit3D *blit3D;
extern Menu menu;

class BlitButton;
class BlitSlider;

BlitUI::BlitUI(float userInterfacePos_X, float userInterfacePos_Y)
{
	UI_Pos.x = userInterfacePos_X;
	UI_Pos.y = userInterfacePos_Y;

	UI_Offset = glm::vec2(0.f, 0.f);
}

BlitButton* BlitUI::newButton(std::string name, float posOffset_x, float posOffset_y, float btnWidth, float btnHeight, void(*input)())
{
	glm::vec2 btnPos = UI_Pos + UI_Offset + glm::vec2(posOffset_x, posOffset_y);

	BlitButton *blitBtn = new BlitButton(name, btnPos.x, btnPos.y, btnWidth, btnHeight, input);

	elementList.push_back(blitBtn);

	return blitBtn;
}

BlitSlider* BlitUI::newSlider(std::string name, float posOffset_x, float posOffset_y, float sliderWidth, float sliderHeight, void(*input)(float))
{
	glm::vec2 sliderPosition = UI_Pos + UI_Offset + glm::vec2(posOffset_x, posOffset_y);

	BlitSlider *blitSlider = new BlitSlider(name, sliderPosition.x, sliderPosition.y, sliderWidth, sliderHeight, input);

	elementList.push_back(blitSlider);

	return blitSlider;
}

BlitLabel* BlitUI::newLabel(AngelcodeFont* _font, std::string _text, float _posOffsetX, float _posOffsetY)
{
	glm::vec2 labelPosition = UI_Pos + UI_Offset + glm::vec2(_posOffsetX, _posOffsetY);

	BlitLabel* blitLabel = new BlitLabel(_font, _text, labelPosition.x, labelPosition.y);

	elementList.push_back(blitLabel);

	return blitLabel;
}

void BlitUI::DeinitUI()
{
	for (BlitElement* &element : elementList)
	{
		switch (element->elementType)
		{
		case ElementType::BUTTON:
			delete ((BlitButton*)element);
			break;
		case ElementType::SLIDER:
			delete ((BlitSlider*)element);
			break;
		case ElementType::LABEL:
			delete ((BlitLabel*)element);
			break;
		}
	}

	elementList.clear();
}

void BlitUI::SetButtonFont(AngelcodeFont *btnTxtFont)
{
	for (BlitElement* &element : elementList)
	{
		if (element->elementType == ElementType::BUTTON)
		{
			((BlitButton*)element)->SetFont(btnTxtFont);
		}
	}
}

void BlitUI::SetButtonSprites(Sprite *simpleBtnSprite, Sprite *hoverBtnSprite, Sprite *pressedBtnSprite)
{
	for (BlitElement* &element : elementList)
	{
		if (element->elementType == ElementType::BUTTON)
		{
			((BlitButton*)element)->SetBtnSprites(simpleBtnSprite, hoverBtnSprite, pressedBtnSprite);
		}
	}
}

void BlitUI::AnimateButtons(bool animated)
{
	for (BlitElement* &element : elementList)
	{
		if (element->elementType == ElementType::BUTTON)
		{
			((BlitButton*)element)->ShowAnimation(animated);
		}
	}
}

void BlitUI::SetSliderFont(AngelcodeFont *sliderTxtFont)
{
	BlitSlider* slider;

	for (BlitElement* &element : elementList)
	{
		if (element->elementType == ElementType::SLIDER)
		{
			slider = ((BlitSlider*)element);

			slider->SetFont(sliderTxtFont);

			slider->SetSliderTextOffset(glm::vec2(0.f, sliderTxtFont->FontHeight()), glm::vec2(15.f, sliderTxtFont->FontHeight() / 3));
		}
	}
}

void BlitUI::SetSliderSprites(Sprite *sliderGuide, Sprite *sliderBtn)
{
	for (BlitElement* &element : elementList)
	{
		if (element->elementType == ElementType::SLIDER)
		{
			((BlitSlider*)element)->SetSliderSprites(sliderGuide, sliderBtn);
		}
	}
}

void BlitUI::SetSliderTextOffset(float nameOffset_x, float nameOffset_y, float valueOffset_x, float valueOffset_y)
{
	for (BlitElement* &element : elementList)
	{
		if (element->elementType == ElementType::SLIDER)
		{
			((BlitSlider*)element)->SetSliderTextOffset(glm::vec2(nameOffset_x, nameOffset_y), glm::vec2(valueOffset_x, valueOffset_y));
		}
	}
}

void BlitUI::UpdateUI(Vector2Int cursorPos)
{
	for (BlitElement* &element : elementList)
	{
		switch (element->elementType)
		{
		case ElementType::BUTTON:
			((BlitButton*)element)->Update(cursorPos);
			break;
		case ElementType::SLIDER:
			((BlitSlider*)element)->Update(cursorPos);
			break;

		//Elements that don't Update
		case ElementType::LABEL:
		default:
			break;
		}
	}
}

void BlitUI::DrawUI()
{
	for (BlitElement* &element : elementList)
	{
		switch (element->elementType)
		{
		case ElementType::BUTTON:
			((BlitButton*)element)->Draw();
			break;
		case ElementType::SLIDER:
			((BlitSlider*)element)->Draw();
			break;
		case ElementType::LABEL:
			((BlitLabel*)element)->Draw();
			break;
		default:
			break;
		}
	}
}

void BlitUI::SetOffset(glm::vec2 _offset)
{
	glm::vec2 newPosition;

	for (BlitElement* &element : elementList)
	{
		//Refactor this
		newPosition = element->BE_Pos - UI_Offset + _offset;

		//Refactor this as well
		element->SetPosition(newPosition.x, newPosition.y);
		
		//Special cases
		if (element->elementType == ElementType::SLIDER)
		{
			((BlitSlider*)element)->offsetSlider(_offset.x - UI_Offset.x);
		}
	}

	UI_Offset = _offset;
}