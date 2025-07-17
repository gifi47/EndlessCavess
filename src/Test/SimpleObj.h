#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <string>
#include <vector>
#include <map>
#include "../Rendering/Texture.h"
#include "../Rendering/Shader.h"
#include "../Utils/Utils.h"

namespace tst {
	class SimpleObj {
	public:
		SimpleObj(std::string modelFileName, Rendering::Shader& shader, Rendering::Texture& texture, int attributeCount);
		void Render(const glm::mat4& projView);
		void RenderMVP(const glm::mat4& MVP);
		void BindInstantModelMatricesBuffer(unsigned int amount, glm::mat4* modelMatrices);
		void Instantiate(glm::mat4& proj, glm::mat4& view);
		void Dispose();

		GLuint VAO;
		GLuint VBO;
		GLuint EBO;
		GLuint IMBO;
		int vertexCount;

		unsigned int amount;

		glm::mat4 modelMatrix;

		Rendering::Shader shader;
		Rendering::Texture texture;

		std::vector<float> vertices;
		std::vector<int> indexes;
	};
}