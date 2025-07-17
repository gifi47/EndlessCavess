#pragma once

#include <GL/glew.h>
#include <string>
#include <vector>
#include "../Rendering/Texture.h"
#include "../Rendering/Shader.h"


namespace Test {
	class Label {
	public:
		Label(int size, Rendering::Texture& font, Rendering::Shader& shader);
		void Render(const GLfloat* MVP);
		void SetValue(int value);
		void Dispose();

		int size;
		GLuint VAO;
		GLuint VBO;
		GLuint EBO;
		int vertexCount;
		std::vector<float> verts;
		std::vector<float> tex;
		std::vector<int> indexes;

		Rendering::Shader shader;
		Rendering::Texture font;
	};
}