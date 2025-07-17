#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>

#include "../Rendering/Texture.h"
#include "../Rendering/Shader.h"
#include "../Rendering/Batch.h"

namespace tst {
	class Orbit {
	public:
		Rendering::Texture texture;
		Rendering::Shader shader;
		Rendering::Shader rockShader;
		Rendering::Batch rock;
		GLuint uniformMVPLocation;
		GLuint uniformModifireLocation;

		Rendering::Batch center;
		glm::mat4 centerModelMatrix;

		unsigned int amount;
		glm::mat4* modelMatrices;

		void Create(Rendering::Batch& centerObj, Rendering::Batch& rockObj, unsigned int amount);
		void Draw(glm::mat4& projView);
	};
}