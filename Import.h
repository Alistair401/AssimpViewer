#pragma once
#include <string>
#include "AnimatedModel.h"

class Import
{
public:
	static AnimatedModel* LoadAnimatedFile(std::string filename);
};
