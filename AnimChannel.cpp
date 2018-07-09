#include "stdafx.h"
#include "AnimChannel.h"
#include <algorithm>

struct _CompareKeys {
	bool operator() (const VectorKey & left, const VectorKey & right)
	{
		return left.time < right.time;
	}
	bool operator() (const VectorKey & left, double right)
	{
		return left.time < right;
	}
	bool operator() (double left, const VectorKey & right)
	{
		return left < right.time;
	}
	bool operator() (const QuaternionKey & left, const QuaternionKey & right)
	{
		return left.time < right.time;
	}
	bool operator() (const QuaternionKey & left, double right)
	{
		return left.time < right;
	}
	bool operator() (double left, const QuaternionKey & right)
	{
		return left < right.time;
	}
} CompareKeys;

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

VectorKey& AnimChannel::GetPositionKey(size_t index)
{
	return position_keys[index];
}

QuaternionKey& AnimChannel::GetRotationKey(size_t index)
{
	return rotation_keys[index];
}

VectorKey& AnimChannel::GetScalingKey(size_t index)
{
	return scaling_keys[index];
}

size_t AnimChannel::GetPositionKeyIndex(double time)
{
	auto it = std::upper_bound(position_keys.begin(), position_keys.end(), time, CompareKeys);
	return it - position_keys.begin();
}

size_t AnimChannel::GetRotationKeyIndex(double time)
{
	auto it = std::upper_bound(rotation_keys.begin(), rotation_keys.end(), time, CompareKeys);
	return it - rotation_keys.begin();
}

size_t AnimChannel::GetScalingKeyIndex(double time)
{
	auto it = std::upper_bound(scaling_keys.begin(), scaling_keys.end(), time, CompareKeys);
	return it - scaling_keys.begin();
}
