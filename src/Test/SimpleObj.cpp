#include "SimpleObj.h"

tst::SimpleObj::SimpleObj(std::string modelFileName, Rendering::Shader& shader, Rendering::Texture& texture, int attributeCount = 9) {
	//std::vector<float> vertices;
	switch (attributeCount)
	{
	case 5:
		ImportObj5(modelFileName, vertices);
		break;
	case 8:
		ImportObj(modelFileName, vertices);
		break;
	case 9:
		ImportObjMultiTex(modelFileName, vertices);
		break;
	default:
		ImportObj(modelFileName, vertices);
		attributeCount = 8;
		break;
	}

	this->shader = shader;
	this->texture = texture;
	this->modelMatrix = glm::mat4(1);

	glGenVertexArrays(1, &this->VAO);
	glGenBuffers(1, &this->VBO);
	glGenBuffers(1, &this->EBO);

	//std::vector<int> indexes(vertices.size() / attributeCount);
	indexes.resize(vertices.size() / attributeCount);
	for (int i = 0; i < indexes.size(); i++) {
		indexes[i] = i;
	}

	this->vertexCount = indexes.size();

	glBindVertexArray(this->VAO);

	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * indexes.size(), indexes.data(), GL_STATIC_DRAW);

	// XYZ координаты
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, attributeCount * sizeof(float), (GLvoid*)nullptr);
	glEnableVertexAttribArray(0);

	// RGB цвет
	if (attributeCount > 7) {
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, attributeCount * sizeof(float), (GLvoid*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
		// Текстурные координаты
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, attributeCount * sizeof(float), (GLvoid*)(6 * sizeof(float)));
		glEnableVertexAttribArray(2);
	}
	else {
		// Текстурные координаты
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, attributeCount * sizeof(float), (GLvoid*)(3 * sizeof(float)));
		glEnableVertexAttribArray(2);
	}



	// ID текстуры
	if (attributeCount == 9) {
		glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, attributeCount * sizeof(float), (GLvoid*)(8 * sizeof(float)));
		glEnableVertexAttribArray(3);
	}

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
}

void tst::SimpleObj::Render(const glm::mat4& projView)
{
	glm::mat4 MVP = projView * modelMatrix;
	this->shader.Use();
	glUniformMatrix4fv(glGetUniformLocation(this->shader.programId, "MVP"), 1, GL_FALSE, &MVP[0][0]);
	this->texture.Use();
	glBindVertexArray(this->VAO);
	glDrawElements(GL_TRIANGLES, vertexCount, GL_UNSIGNED_INT, nullptr);
	glBindVertexArray(0);
}

void tst::SimpleObj::RenderMVP(const glm::mat4& MVP)
{
	this->shader.Use();
	glUniformMatrix4fv(glGetUniformLocation(this->shader.programId, "MVP"), 1, GL_FALSE, &MVP[0][0]);
	this->texture.Use();
	glBindVertexArray(this->VAO);
	glDrawElements(GL_TRIANGLES, vertexCount, GL_UNSIGNED_INT, nullptr);
	glBindVertexArray(0);
}

void tst::SimpleObj::BindInstantModelMatricesBuffer(unsigned int amount, glm::mat4* modelMatrices)
{
	this->amount = amount;

	glGenBuffers(1, &IMBO);

	glBindBuffer(GL_ARRAY_BUFFER, IMBO);
	glBufferData(GL_ARRAY_BUFFER, amount * sizeof(glm::mat4), &modelMatrices[0], GL_STATIC_DRAW);
	glBindVertexArray(VAO);
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (GLvoid*)0);

	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (GLvoid*)(1 * sizeof(glm::vec4)));

	glEnableVertexAttribArray(5);
	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (GLvoid*)(2 * sizeof(glm::vec4)));

	glEnableVertexAttribArray(6);
	glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (GLvoid*)(3 * sizeof(glm::vec4)));

	glVertexAttribDivisor(3, 1);
	glVertexAttribDivisor(4, 1);
	glVertexAttribDivisor(5, 1);
	glVertexAttribDivisor(6, 1);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void tst::SimpleObj::Instantiate(glm::mat4& proj, glm::mat4& view)
{
	shader.Use();
	glUniformMatrix4fv(glGetUniformLocation(shader.programId, "projection"), 1, GL_FALSE, &proj[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(shader.programId, "view"), 1, GL_FALSE, &view[0][0]);
	texture.Use();

	glBindVertexArray(VAO);
	glDrawElementsInstanced(GL_TRIANGLES, static_cast<unsigned int>(indexes.size()), GL_UNSIGNED_INT, nullptr, amount);
	glBindVertexArray(0);
}

void tst::SimpleObj::Dispose()
{
	glDeleteBuffers(1, &this->VAO);
	glDeleteBuffers(1, &this->VBO);
	glDeleteBuffers(1, &this->EBO);
}
