#pragma once
#include "Animation.h"

using Pose = std::unordered_map<std::string, glm::mat4>;

class AnimationEvaluator
{
public:
	static Pose Evaluate(Animation& animation, double time);
};
