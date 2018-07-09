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
	channel_mapping[channel->name] = channels.size();
	channels.emplace_back(std::move(channel));
}

bool Animation::HasChannel(std::string name)
{
	auto found = channel_mapping.find(name);
	return (found != channel_mapping.end());
}

AnimChannel& Animation::GetChannel(std::string name)
{
	auto found = channel_mapping.find(name);

	assert(found != channel_mapping.end());

	return *channels[found->second];
}

void Animation::SetDuration(double ticks)
{
	this->duration = ticks;
}

double Animation::GetDuration()
{
	return duration;
}

void Animation::ForEachChannel(const std::function<void(AnimChannel&)>& f)
{
	for (auto&& channel : channels) {
		f(*channel);
	}
}

std::string Animation::GetName()
{
	return name;
}
