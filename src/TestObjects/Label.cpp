#include "Label.h"


Test::Label::Label(int size, Rendering::Texture& font, Rendering::Shader& shader) {
	this->size = size;
	this->font = font;
	this->shader = shader;

	this->verts.resize(size * 8);
	int k = 0;
	for (int i = 0; i < verts.size(); i += 8) {
		verts[i] = -1 + k; verts[i + 1] = 1;
		verts[i + 2] = -1 + k; verts[i + 3] = -1;
		verts[i + 4] = 1 + k; verts[i + 5] = -1;
		verts[i + 6] = 1 + k; verts[i + 7] = 1;
		k += 2;
	}
	this->tex.resize(size * 8);
	for (int i = 0; i < tex.size(); i += 8) {
		tex[i] = 0; tex[i + 1] = 1;
		tex[i + 2] = 0; tex[i + 3] = 0;
		tex[i + 4] = 0.1; tex[i + 5] = 0;
		tex[i + 6] = 0.1; tex[i + 7] = 1;
	}

	this->indexes.resize(size * 6);
	k = 0;
	for (int i = 0; i < indexes.size(); i+=6) {
		indexes[i] = 0 + k;
		indexes[i + 1] = 1 + k;
		indexes[i + 2] = 2 + k;
		indexes[i + 3] = 2 + k;
		indexes[i + 4] = 3 + k;
		indexes[i + 5] = 0 + k;
		k += 4;
	}

	glGenVertexArrays(1, &this->VAO);
	glGenBuffers(1, &this->VBO);
	glGenBuffers(1, &this->EBO);
	this->vertexCount = indexes.size();

	glBindVertexArray(this->VAO);

	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * verts.size() * tex.size(), nullptr, GL_DYNAMIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * verts.size(), verts.data());
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(float) * verts.size(), sizeof(float) * tex.size(), tex.data());

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * indexes.size(), indexes.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (GLvoid*)nullptr);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (GLvoid*)(verts.size() * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);
}

void Test::Label::Render(const GLfloat* MVP) {
	this->shader.Use();
	glUniformMatrix4fv(glGetUniformLocation(this->shader.programId, "MVP"), 1, GL_FALSE, MVP);
	this->font.Use();
	glBindVertexArray(this->VAO);
	glDrawElements(GL_TRIANGLES, vertexCount, GL_UNSIGNED_INT, nullptr);
	glBindVertexArray(0);
}

void Test::Label::SetValue(int value) {
	std::string str = std::to_string(value);
	int k = str.size() - size;
	for (int i = 0; i < tex.size(); i += 8) {
		float left = (k < 0 ? 0 : (str[k] - '0') * 0.1);
		k++;
		tex[i] = left; tex[i + 1] = 1;
		tex[i + 2] = left; tex[i + 3] = 0;
		tex[i + 4] = left + 0.1; tex[i + 5] = 0;
		tex[i + 6] = left + 0.1; tex[i + 7] = 1;
	}

	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(float) * size * 8, sizeof(float) * tex.size(), tex.data());
}

void Test::Label::Dispose() {
	glDeleteBuffers(1, &this->VAO);
	glDeleteBuffers(1, &this->VBO);
	glDeleteBuffers(1, &this->EBO);
}