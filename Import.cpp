#include "stdafx.h"
#include "Import.h"
#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>
#include <unordered_map>

static glm::mat4 AIToGLMMat4(aiMatrix4x4 ai_mat) {
	glm::mat4 result;
	aiMatrix4x4 transposed = ai_mat.Transpose();
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++)
		{
			result[i][j] = transposed[i][j];
		}
	}
	return result;
}

void ProcessVertices(Mesh* mesh, aiMesh* ai_mesh) {
	for (size_t i = 0; i < ai_mesh->mNumVertices; i++)
	{
		MeshVertex mesh_vertex = {
			glm::vec3(0.0),
			glm::vec3(0.0),
			glm::vec3{ 1,0,1 },
			glm::vec2(0.0),
			glm::uvec4{ 0,0,0,0 },
			glm::vec4{ 0.0,0.0,0.0,0.0 }
		};

		if (ai_mesh->HasPositions()) {
			aiVector3D ai_pos = ai_mesh->mVertices[i];
			glm::vec4 position = glm::vec4(ai_pos.x, ai_pos.y, ai_pos.z, 1.0f);
			mesh_vertex.position = { position.x, position.y, position.z };
		}

		if (ai_mesh->HasNormals()) {
			aiVector3D ai_norm = ai_mesh->mNormals[i];
			mesh_vertex.normal = { ai_norm.x,ai_norm.y,ai_norm.z };
		}

		if (ai_mesh->HasVertexColors(0)) {
			aiColor4D ai_colour = ai_mesh->mColors[0][i];
			mesh_vertex.colour = { ai_colour.r,ai_colour.g, ai_colour.b };
		}

		if (ai_mesh->HasTextureCoords(0)) {
			aiVector3D ai_uv = ai_mesh->mTextureCoords[0][i];
			mesh_vertex.uv = { ai_uv.x, ai_uv.y };
		}

		mesh->AddVertex(mesh_vertex);
	}
}

void ProcessFaces(Mesh* mesh, aiMesh* ai_mesh) {
	for (size_t i = 0; i < ai_mesh->mNumFaces; i++)
	{
		aiFace ai_face = ai_mesh->mFaces[i];
		for (size_t j = 0; j < ai_face.mNumIndices; j++)
		{
			mesh->AddIndex(ai_face.mIndices[j]);
		}
	}
}

void ProcessBones(AnimatedModel* model, Mesh* mesh, aiMesh* ai_mesh) {
	std::unordered_map<unsigned int, unsigned int> weight_count;
	for (unsigned int i = 0; i < ai_mesh->mNumBones; i++)
	{
		aiBone* ai_bone = ai_mesh->mBones[i];

		Bone* bone = new Bone{ ai_bone->mName.C_Str(), AIToGLMMat4(ai_bone->mOffsetMatrix) };
		mesh->AddBone(bone);

		for (size_t j = 0; j < ai_bone->mNumWeights; j++)
		{
			aiVertexWeight ai_weight = ai_bone->mWeights[j];
			if (weight_count.find(ai_weight.mVertexId) == weight_count.end()) {
				weight_count[ai_weight.mVertexId] = 0;
			}

			if (weight_count[ai_weight.mVertexId] < 4) {
				mesh->AddWeight(ai_weight.mVertexId, weight_count[ai_weight.mVertexId]++, i, ai_weight.mWeight);
			}
		}

		model->RegisterBone(bone);
	}
}

Mesh* ProcessMesh(AnimatedModel* model, aiMesh* ai_mesh) {
	Mesh* mesh = new Mesh();
	
	ProcessVertices(mesh, ai_mesh);

	ProcessFaces(mesh, ai_mesh);

	if (ai_mesh->HasBones()) {
		ProcessBones(model, mesh, ai_mesh);
	}

	return mesh;
}

Node* ExploreHeirarchy(AnimatedModel* model, aiNode* ai_node, const aiScene* ai_scene, glm::mat4 parent_transform) {
	Node* node = new Node(ai_node->mName.C_Str());

	glm::mat4 relative_transform = AIToGLMMat4(ai_node->mTransformation);
	glm::mat4 node_transform = parent_transform * relative_transform;

	node->SetTransform(relative_transform);

	for (unsigned int mesh_index = 0; mesh_index < ai_node->mNumMeshes; mesh_index++)
	{
		aiMesh* ai_mesh = ai_scene->mMeshes[ai_node->mMeshes[mesh_index]];
		node->AddMesh(ProcessMesh(model, ai_mesh));
	}

	for (unsigned int child_index = 0; child_index < ai_node->mNumChildren; child_index++) {
		node->AddChild(ExploreHeirarchy(model, ai_node->mChildren[child_index], ai_scene, node_transform));
	}

	return node;
}

void ProcessAnimations(AnimatedModel* model, const aiScene* ai_scene) {
	for (size_t i = 0; i < ai_scene->mNumAnimations; i++)
	{
		aiAnimation* ai_animation = ai_scene->mAnimations[i];

		Animation* animation = new Animation();
		animation->SetName(ai_animation->mName.C_Str());
		animation->SetTickRate(ai_animation->mTicksPerSecond);
		animation->SetDuration(ai_animation->mDuration);

		for (size_t j = 0; j < ai_animation->mNumChannels; j++)
		{
			aiNodeAnim* ai_channel = ai_animation->mChannels[j];

			AnimChannel* channel = new AnimChannel();
			channel->SetName(ai_channel->mNodeName.C_Str());

			for (size_t k = 0; k < ai_channel->mNumPositionKeys; k++)
			{
				aiVectorKey ai_key = ai_channel->mPositionKeys[k];
				VectorKey position_key = { ai_key.mTime ,{ai_key.mValue.x,ai_key.mValue.y,ai_key.mValue.z } };
				channel->AddPositionKey(position_key);
			}

			for (size_t k = 0; k < ai_channel->mNumRotationKeys; k++)
			{
				aiQuatKey ai_key = ai_channel->mRotationKeys[k];
				QuaternionKey rotation_key = { ai_key.mTime,{ ai_key.mValue.w, ai_key.mValue.x,ai_key.mValue.y,ai_key.mValue.z  } };
				channel->AddRotationKey(rotation_key);
			}

			for (size_t k = 0; k < ai_channel->mNumScalingKeys; k++)
			{
				aiVectorKey ai_key = ai_channel->mScalingKeys[k];
				VectorKey scaling_key = { ai_key.mTime ,{ ai_key.mValue.x,ai_key.mValue.y,ai_key.mValue.z } };
				channel->AddScalingKey(scaling_key);
			}

			animation->AddChannel(channel);
		}

		std::cout << "Found animation: " << animation->GetName() << std::endl;
		model->AddAnimation(animation);
	}
}

AnimatedModel* ProcessScene(const aiScene* ai_scene) {
	AnimatedModel* model = new AnimatedModel();

	ProcessAnimations(model, ai_scene);

	Node* root_node = ExploreHeirarchy(model, ai_scene->mRootNode, ai_scene, glm::mat4(1.0f));

	model->SetRoot(root_node);

	model->SetInverseRootTransform(AIToGLMMat4(ai_scene->mRootNode->mTransformation));

	return model;
}

AnimatedModel* Import::LoadAnimatedFile(std::string filename)
{
	Assimp::Importer importer;

	const aiScene* ai_scene = importer.ReadFile(filename, aiProcess_Triangulate);

	if (ai_scene == NULL) {
		return nullptr;
	}

	AnimatedModel* model = ProcessScene(ai_scene);

	return model;
}
