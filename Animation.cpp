#include "stdafx.h"
#include "Animation.h"

void Animation::SetName(std::string name)
{
	this->name = name;
}

void Animation::SetTickRate(double rate)
{
	tick_rate = rate;
}

double Animation::GetTickRate()
{
	return tick_rate;
}

void Animation::AddChannel(AnimChannel * channel)
{
	channels[channel->GetName()] = channel;
}

AnimChannel * Animation::GetChannel(std::string name)
{
	if (channels.find(name) == channels.end()) {
		return nullptr;
	}
	return channels[name];
}

std::string Animation::GetName()
{
	return name;
}
