#ifndef TEXTURE_H
#define TEXTURE_H

#include <GL/glew.h>
#include <string>
#include <SOIL/SOIL.h>

//#include "interface_idisposable.h"
#include "IDisposable.h"

namespace Rendering {
	class Texture : IDisposable {
	public:
		Texture();
		Texture(const std::string& filePath, int imageType);
		Texture(const Texture& texture);
		Texture(Texture&& texture);

		Texture& operator=(const Texture& other);
		Texture& operator=(Texture&& other);

		void Use();
		void Use(int id);
		void Initialize(const std::string& filePath, int imageType);
		
		GLuint textureId{};
	protected:
		void OnDispose() final override;
	};
}

#endif