#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>
#include <string>
#include <iostream>
#include <vector>
#include "../Utils/Utils.h"

//#include "interface_idisposable.h"
#include "IDisposable.h"

namespace Rendering {
	class Shader : IDisposable {
	public:
		Shader();
		Shader(const std::string& name);
		Shader(const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilePath);
		Shader(const Shader& shader);
		Shader(Shader&& shader);

		Shader& operator=(const Shader& other);
		Shader& operator=(Shader&& other);

		void Use();
		void Initialize(const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilePath);

		GLuint programId{};

		static void CheckShaderErrors(const GLuint& shaderId);
		static void CheckProgramErrors(const GLuint& programId);

	protected:
		void OnDispose() final override;
	};

}

#endif