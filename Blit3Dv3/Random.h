#pragma once
#include <random>

class Random
{
public:

	//Mersenne Twister
	std::mt19937 rng;

	Random(); //Constructor that seeds the rng with a std::random_device	
	short Random::genTileValue(short traversableRatio, short obstacleRatio); //Generates either a traversable or obstacle tile of traversableRatio to obstacleRatio tiles
};