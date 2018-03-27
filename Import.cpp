#include "stdafx.h"
#include "Import.h"
#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>

static glm::mat4 AIToGLMMat4(aiMatrix4x4 ai_mat) {
	glm::mat4 result;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++)
		{
			result[i][j] = ai_mat[i][j];
		}
	}
	return result;
}

Node* ExploreHeirarchy(aiNode* ai_node, const aiScene* ai_scene, glm::mat4 parent_transform) {
	Node* node = new Node(ai_node->mName.C_Str());
	glm::mat4 relative_transform = AIToGLMMat4(ai_node->mTransformation.Transpose());
	glm::mat4 transform = parent_transform * relative_transform;

	for (unsigned int mesh_index = 0; mesh_index < ai_node->mNumMeshes; mesh_index++)
	{
		aiMesh* ai_mesh = ai_scene->mMeshes[ai_node->mMeshes[mesh_index]];

		Mesh* mesh = new Mesh();

		for (size_t i = 0; i < ai_mesh->mNumVertices; i++)
		{
			MeshVertex mesh_vertex = { glm::vec3(0.0), glm::vec3(0.0), glm::vec3{1,0,1}, glm::vec2(0.0) };

			if (ai_mesh->HasPositions()) {
				aiVector3D ai_pos = ai_mesh->mVertices[i];
				glm::vec4 transformed_position = transform * glm::vec4{ ai_pos.x,ai_pos.y,ai_pos.z,1 };
				mesh_vertex.position = {transformed_position.x,transformed_position.y,transformed_position.z};
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

		for (size_t i = 0; i < ai_mesh->mNumFaces; i++)
		{
			aiFace ai_face = ai_mesh->mFaces[i];
			for (size_t j = 0; j < ai_face.mNumIndices; j++)
			{
				mesh->AddIndex(ai_face.mIndices[j]);
			}
		}

		mesh->GenBuffers();

		node->AddMesh(mesh);
	}

	for (unsigned int child_index = 0; child_index < ai_node->mNumChildren; child_index++) {
		node->AddChild(ExploreHeirarchy(ai_node->mChildren[child_index], ai_scene, transform));
	}

	return node;
}

Model* ProcessScene(const aiScene* ai_scene) {
	Model* model = new Model();

	model->SetRoot(ExploreHeirarchy(ai_scene->mRootNode, ai_scene, glm::mat4(1)));

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
