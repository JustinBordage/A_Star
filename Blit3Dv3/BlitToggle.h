#pragma once
#include "BlitElement.h"

class BlitToggle : public BlitElement
{
public:
	void ToggleCollider(Vector2Int cursorPos);

	void Toggler();

	void UpdateToggle(Vector2Int cursorPos);

	void DrawToggle();
};