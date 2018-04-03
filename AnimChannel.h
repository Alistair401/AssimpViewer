#pragma once
#include <vector>
#include "glm\glm.hpp"
#include "glm\gtc\quaternion.hpp"

struct VectorKey {
	double time;
	glm::vec3 value;
};

struct QuaternionKey {
	double time;
	glm::quat value;
};

class AnimChannel
{
public:
	void SetName(std::string name);
	std::string GetName();
	void AddPositionKey(VectorKey key);
	void AddRotationKey(QuaternionKey key);
	void AddScalingKey(VectorKey key);
private:
	std::string name;
	std::vector<VectorKey> scaling_keys;
	std::vector<QuaternionKey> rotation_keys;
	std::vector<VectorKey> position_keys;
};

