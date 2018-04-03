#pragma once
#include <string>
#include "Model.h"

class Import
{
public:
	static Model* LoadFile(std::string filename);
};
