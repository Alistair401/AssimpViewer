#pragma once
#include "AnimChannel.h"
#include <unordered_map>

class Animation
{
public:
	void SetName(std::string name);
	std::string GetName();
	void SetTickRate(double rate);
	double GetTickRate();
	void AddChannel(AnimChannel* channel);
	AnimChannel* GetChannel(std::string name);
	void SetDuration(double ticks);
	double GetDuration();
private:
	std::string name;
	double tick_rate;
	double duration;
	std::unordered_map<std::string, AnimChannel*> channels;
};

