#include "stdafx.h"
#include "AnimationEvaluator.h"

#include <cmath>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\quaternion.hpp>

void AnimationEvaluator::Evaluate(Animation & animation, double time)
{
	double tick = time * animation.GetTickRate();
	
	tick = std::fmod(tick, animation.GetDuration());
}
