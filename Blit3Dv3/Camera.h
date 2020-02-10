#pragma once

/* Example Camera class for panning and screenshake effects*/

#include "Blit3D.h"
extern Blit3D *blit3D;

#include<atomic>
#include <random>

class Camera2D
{
private:
	std::mt19937 rng;

	//screenshake variables	
	glm::vec3 shakeOffset; //use this to shake the camera
	float angle;
	float amount;
	bool shaking;
	float shakeTimer;
	glm::vec3 panOffset; //use this to pan the camera around
	glm::vec3 moveDir; //direction to pan in
	float halfScreenWidth, halfScreenHeight;

public:
	//set it's valid pan area
	//the center of the camera will be restricted inside these bounds
	float minX, minY, maxX, maxY, minZ, maxZ; //min and max offsets in each direction
	std::atomic<int> posX, posY, posZ; //use these to do whole pixel panning
	float panSpeed; //how fast we pan in pixels/seconds when using the Pan() method

	//screenshake variables	
	float shakeReductionFactor;//scales the shake amount down every 1/60th of a second

	Camera2D();

	//Pan() moves us in the direction of moveDir,
	//at panSpeed length per second.
	void Pan(float xDir, float yDir);

	//PanTo moves us to the pan location provided
	void PanTo(float pos_X, float pos_Y);

	void Shake(float radiusInPixels);

	void StopShaking();

	void Update(float seconds);

	void Draw();

	void UnDraw();
};