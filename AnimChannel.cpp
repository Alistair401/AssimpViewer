#include "stdafx.h"
#include "AnimChannel.h"

void AnimChannel::SetName(std::string name)
{
	this->name = name;
}

std::string AnimChannel::GetName()
{
	return name;
}

void AnimChannel::AddPositionKey(VectorKey key)
{
	position_keys.push_back(key);
}

void AnimChannel::AddRotationKey(QuaternionKey key)
{
	rotation_keys.push_back(key);
}

void AnimChannel::AddScalingKey(VectorKey key)
{
	scaling_keys.push_back(key);
}
