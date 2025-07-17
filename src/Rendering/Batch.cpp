#include "Batch.h"

Rendering::Batch::Batch(std::vector<float>& vertexData, int vertexSize){
	std::vector<int> indexes(vertexData.size() / vertexSize);
	for (int i = 0; i < indexes.size(); i++) indexes[i] = i;
	Initialize(vertexData, indexes);
}

Rendering::Batch::Batch(std::vector<float>& vertexData, std::vector<int>& indexes){
	Initialize(vertexData, indexes);
}

Rendering::Batch::Batch(const Batch& batch){
	this->VAO = batch.VAO;
	this->VBO = batch.VBO;
	this->EBO = batch.EBO;
	this->vertexCount = batch.vertexCount;
}

void Rendering::Batch::Use(){
	glBindVertexArray(this->VAO);
	glDrawElements(GL_TRIANGLES, vertexCount, GL_UNSIGNED_INT, nullptr);
	glBindVertexArray(0);
}

void Rendering::Batch::Initialize(std::vector<float>& vertexData, std::vector<int>& indexes){
	glGenVertexArrays(1, &this->VAO);
	glGenBuffers(1, &this->VBO);
	glGenBuffers(1, &this->EBO);
	this->vertexCount = indexes.size();

	glBindVertexArray(this->VAO);

	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertexData.size(), vertexData.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * indexes.size(), indexes.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (GLvoid*)nullptr);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (GLvoid*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (GLvoid*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);


	/*glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, color));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, texture_coord));
	glEnableVertexAttribArray(2);

	glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, texture_id));
	glEnableVertexAttribArray(3);

	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, normal));
	glEnableVertexAttribArray(4);*/

	glBindVertexArray(0);
}

void Rendering::Batch::Dispose(){
	glDeleteBuffers(1, &this->VAO);
	glDeleteBuffers(1, &this->VBO);
	glDeleteBuffers(1, &this->EBO);
}