#pragma once
#include "AnimChannel.h"

#include <unordered_map>
#include <memory>
#include <functional>

class Animation
{
public:
	void SetName(std::string name);
	std::string GetName();
	void SetTickRate(double rate);
	double GetTickRate();
	void AddChannel(AnimChannel* channel);
	bool HasChannel(std::string name);
	AnimChannel& GetChannel(std::string name);
	void SetDuration(double ticks);
	double GetDuration();
	void ForEachChannel(const std::function<void(AnimChannel&)>& f);
private:
	std::string name;
	double tick_rate;
	double duration;
	std::unordered_map<std::string, size_t> channel_mapping;
	std::vector<std::unique_ptr<AnimChannel>> channels;
};
