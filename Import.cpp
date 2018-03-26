#include "stdafx.h"
#include "Import.h"
#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>

Node* ExploreHeirarchy(aiNode* ai_node, const aiScene* ai_scene) {
	Node* node = new Node(ai_node->mName.C_Str());

	for (unsigned int mesh_index = 0; mesh_index < ai_node->mNumMeshes; mesh_index++)
	{
		Mesh* mesh = new Mesh();
		aiMesh* ai_mesh = ai_scene->mMeshes[ai_node->mMeshes[mesh_index]];

		node->AddMesh(mesh);
	}

	for (unsigned int child_index = 0; child_index < ai_node->mNumChildren; child_index++) {
		node->AddChild(ExploreHeirarchy(ai_node->mChildren[child_index],ai_scene));
	}

	return node;
}

Model* ProcessScene(const aiScene* ai_scene) {
	Model* model = new Model();

	model->SetRoot(ExploreHeirarchy(ai_scene->mRootNode, ai_scene));

	return model;
}

Model* Import::LoadFile(std::string filename)
{
	Assimp::Importer importer;

	const aiScene* ai_scene = importer.ReadFile(filename,
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices);

	if (ai_scene == NULL) {
		return nullptr;
	}

	return ProcessScene(ai_scene);
}
