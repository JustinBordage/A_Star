#include "Random.h"
#include "Definitions.h"


Random::Random()
{
	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	//Method			:	Random
	//
	//Method parameters	:	none
	//
	//Method return		:	void
	//
	//Synopsis			:   Constructor that seeds the rng with a std::random_device
	//						
	//
	//Modifications		:
	//						Date			Developer				Notes
	//						----			---------				-----
	//						10/17/2018		J. Bordage				Initial Setup
	//																
	//
	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	
	std::random_device rd;																						//Creates a random device
	
	rng.seed(rd());																								//Generates the mersenne twister seed using the random device
}

short Random::genTileValue(short tRatio, short oRatio)
{
	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	//Method			:	genTileValue
	//
	//Method parameters	:	short tRatio, short oRatio
	//
	//Method return		:	short
	//
	//Synopsis			:   Generates tiles based on the passed in tile ratios, tRatio to oRatio
	//						
	//
	//Modifications		:
	//						Date			Developer				Notes
	//						----			---------				-----
	//						10/17/2018		J. Bordage				Initial Setup
	//						10/17/2018		J. Bordage				Added traverable ratio parameter
	//																
	//
	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

	tRatio = -(tRatio - 1);																						//calculates the traversable ratio offset

	std::uniform_int_distribution<int> dN(tRatio, oRatio);														//Generates a number with a tile ratio 'tRatio:oRatio'

	if (dN(rng) <= 0)
		return 0;																								//Return EASY_PATH
	else
		return (short) TileType::WALL;																			//Return WALL
}