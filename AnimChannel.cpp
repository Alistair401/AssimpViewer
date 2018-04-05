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

size_t AnimChannel::NumPositionKeys()
{
	return position_keys.size();
}

size_t AnimChannel::NumRotationKeys()
{
	return rotation_keys.size();
}

size_t AnimChannel::NumScalingKeys()
{
	return scaling_keys.size();
}

VectorKey AnimChannel::GetPositionKey(size_t index)
{
	return position_keys[index];
}

QuaternionKey AnimChannel::GetRotationKey(size_t index)
{
	return rotation_keys[index];
}

VectorKey AnimChannel::GetScalingKey(size_t index)
{
	return scaling_keys[index];
}

size_t AnimChannel::GetPositionKeyIndex(double time)
{
	for (size_t i = 0; i < position_keys.size(); i++)
	{
		if (position_keys[i].time > time) {
			return i;
		}
	}
	return position_keys.size() - 1;
}

size_t AnimChannel::GetRotationKeyIndex(double time)
{
	for (size_t i = 0; i < rotation_keys.size(); i++)
	{
		if (rotation_keys[i].time > time) {
			return i;
		}
	}
	return rotation_keys.size() - 1;
}

size_t AnimChannel::GetScalingKeyIndex(double time)
{
	for (size_t i = 0; i < scaling_keys.size(); i++)
	{
		if (scaling_keys[i].time > time) {
			return i;
		}
	}
	return scaling_keys.size() - 1;
}
