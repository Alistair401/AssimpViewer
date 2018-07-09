#include "stdafx.h"
#include "Renderer.h"

void GenBufferHierarchy(Model& model, ModelNode& node) {
	node.ForEachMesh([&](Mesh& mesh) {
		glGenBuffers(1, &mesh.vbo);
		glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);
		glBufferData(GL_ARRAY_BUFFER, mesh.NumVertices() * sizeof(MeshVertex), &mesh.vertices[0], GL_STATIC_DRAW);

		glGenBuffers(1, &mesh.ibo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.NumIndices() * sizeof(GLuint), &mesh.indices[0], GL_STATIC_DRAW);

		GLuint ssbo_index = glGetProgramResourceIndex(model.shader->ID(), GL_SHADER_STORAGE_BLOCK, "bone_buffer");
		glGenBuffers(1, &mesh.bbo);
		glShaderStorageBlockBinding(model.shader->ID(), ssbo_index, mesh.bbo_binding);
	});

	node.ForEachChild([&](ModelNode& child) {
		GenBufferHierarchy(model, child);
	});
}

void RenderHierarchy(AnimatedModel& model, ModelNode& node) {
	node.ForEachMesh([&](Mesh& mesh) {
		std::vector<glm::mat4> bone_tranforms;
		for (size_t bone_index : mesh.bones) {
			Bone& bone = model.GetBone(bone_index);
			bone_tranforms.push_back(bone.transform);
		}

		glBindBuffer(GL_SHADER_STORAGE_BUFFER, mesh.bbo);
		glBufferData(GL_SHADER_STORAGE_BUFFER, mesh.NumBones() * sizeof(glm::mat4), &bone_tranforms[0], GL_STATIC_DRAW);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, mesh.bbo_binding, mesh.bbo);

		GLsizei stride = sizeof(MeshVertex);

		glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);

		glEnableVertexAttribArray(0); // pos xyz
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, 0);

		glEnableVertexAttribArray(1); // norm xyz
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (const void*)(sizeof(GLfloat) * 3));

		glEnableVertexAttribArray(2); // color rgb
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (const void*)(sizeof(GLfloat) * 6));

		glEnableVertexAttribArray(3); // tex uv
		glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, stride, (const void*)(sizeof(GLfloat) * 9));

		glEnableVertexAttribArray(4); // bone_ids abcd
		glVertexAttribIPointer(4, 4, GL_UNSIGNED_INT, stride, (const void*)(sizeof(GLfloat) * 11));

		glEnableVertexAttribArray(5); // bone_weights abcd
		glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, stride, (const void*)((sizeof(GLfloat) * 11) + sizeof(GLuint) * 4));

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.ibo);

		glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(mesh.NumIndices()), GL_UNSIGNED_INT, 0);
	});

	node.ForEachChild([&](ModelNode& child) {
		RenderHierarchy(model, child);
	});
}

void Renderer::Render(AnimatedModel& model)
{
	model.shader->Use();
	if (!model.IsBuffered()) {
		GenBufferHierarchy(model, *(model.root));
		model.SetBuffered(true);
	}
	RenderHierarchy(model, *(model.root));
}
