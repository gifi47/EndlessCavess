#pragma once

#include <GL/glew.h>
#include <vector>

namespace Rendering {
	class Batch {
	public:
		Batch() = default;
		Batch(std::vector<float>& vertexData, int vertexSize);
		Batch(std::vector<float>& vertexData, std::vector<int>& indexes);
		Batch(const Batch& batch);

		void Use();
		void Initialize(std::vector<float>& vertexData, std::vector<int>& indexes);
		void Dispose();

		GLuint VAO{};
		GLuint VBO{};
		GLuint EBO{};
		int vertexCount{};
	};
}