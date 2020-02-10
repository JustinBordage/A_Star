#include "BlitToggle.h"

extern bool LMB;

void BlitToggle::ToggleCollider(Vector2Int cursorPos)
{

	Collision(cursorPos);


	//Redefine BlitStatus and the sprites so that it can exist without the hoversprite
	//if (BE_State == BlitStatus::IN_FOCUS)
	//{
		//BE_State == BlitStatus::OUT_OF_FOCUS;
	//}
}

void BlitToggle::Toggler()
{

}

void BlitToggle::UpdateToggle(Vector2Int cursorPos)
{
	ToggleCollider(cursorPos);



}

void BlitToggle::DrawToggle()
{

}
