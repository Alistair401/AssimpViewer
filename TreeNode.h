#pragma once

#include <vector>
#include <memory>
#include <functional>

template<typename T>
class TreeNode
{
public:
	TreeNode() {};

	T data;

	void AddChild(TreeNode* child) {
		children.emplace_back(std::move(child));
	};

	void Traverse(std::function<void(TreeNode* parent, TreeNode* node)> transformation) {
		transformation(nullptr, this);
		for (auto& child : children) {
			child.Traverse(transformation, this);
		}
	};
private:
	std::vector<std::unique_ptr<TreeNode>> children;

	void Traverse(std::function<void(TreeNode* parent, TreeNode* node)> transformation, TreeNode* parent) {
		transformation(parent, this);
		for (auto& child : children) {
			child.Traverse(transformation, this);
		}
	};
};


