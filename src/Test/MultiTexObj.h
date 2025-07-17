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
	class MultiTexObj {
	public:
		MultiTexObj(std::string modelFileName, Rendering::Shader& shader);
		MultiTexObj(const std::vector<float>& vertices, const std::vector<int>& indexes, Rendering::Shader& shader);
		void Render(const glm::mat4& projView);
		void Dispose();

		GLuint VAO;
		GLuint VBO;
		GLuint EBO;
		int vertexCount;

		glm::mat4 modelMatrix;

		Rendering::Shader shader;
		Rendering::Texture texture0;
		Rendering::Texture texture1;
		Rendering::Texture texture2;
		Rendering::Texture texture3;
	};
}