#include "Object.h"

Rendering::Object::Object(std::vector<float>& vertexData, std::vector<int>& indexes, const std::string& vertexShaderFilePath,
						  const std::string& fragmentShaderFilePath, const std::string& textureFilePath, int imageType, glm::mat4& modelMatrix){
	Initialize(vertexData, indexes, vertexShaderFilePath, fragmentShaderFilePath, textureFilePath, imageType, modelMatrix);
}

void Rendering::Object::Render(){

}

void Rendering::Object::Initialize(std::vector<float>& vertexData, std::vector<int>& indexes, const std::string& vertexShaderFilePath,
								   const std::string& fragmentShaderFilePath, const std::string& textureFilePath, int imageType, glm::mat4& modelMatrix){
	this->batch = Batch(vertexData, indexes);
	this->shader = Shader(vertexShaderFilePath, fragmentShaderFilePath);
	this->texture = Texture(textureFilePath, imageType);
	this->modelMatrix = modelMatrix;
}

void Rendering::Object::Dispose(){
	this->batch.Dispose();
	//this->shader.Dispose();
	//this->texture.Dispose();
}
