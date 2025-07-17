#pragma once

#include <glm/glm.hpp>

#include "Batch.h"
#include "Shader.h"
#include "Texture.h"
#include <vector>
#include <string>

namespace Rendering {
	class Object {
	public:
		Object() = default;
		Object(std::vector<float>& vertexData, std::vector<int>& indexes, const std::string& vertexShaderFilePath,
			const std::string& fragmentShaderFilePath, const std::string& textureFilePath, int imageType, glm::mat4& modelMatrix);

		void Render();
		void Initialize(std::vector<float>& vertexData, std::vector<int>& indexes, const std::string& vertexShaderFilePath,
			const std::string& fragmentShaderFilePath, const std::string& textureFilePath, int imageType, glm::mat4& modelMatrix);
		void Dispose();

		Batch batch{};
		Texture texture{};
		Shader shader{};
		glm::mat4 modelMatrix{};
	};
}