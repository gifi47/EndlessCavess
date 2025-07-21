#pragma once
/*
	Contains old code from other projects
	TODO: move out useful classes in their own separate files and remove old unused code
*/
#ifndef MY_GRAPHICS_EXTENSIONS_H
#define MY_GRAPHICS_EXTENSIONS_H

#include <vector>
#include <string>
#define GLEW_STATIC
#include <GL/glew.h>

#include "Rendering/Shader.h"
#include "Rendering/Batch.h"
#include "Rendering/Object.h"
#include "TestObjects/Label.h"
#include "Test/SimpleObj.h"
#include "Test/MultiTexObj.h"

void SimpleObjRenderMVP(tst::SimpleObj& m_obj, Rendering::Texture& texture, const glm::mat4& MVP);

void SimpleObjRenderMVP(tst::SimpleObj& m_obj, Rendering::Texture& texture, float glide, const glm::mat4& MVP);

void SetUniform(Rendering::Shader& shader, std::string uniformName, float value);

void SetUniform(Rendering::Shader& shader, std::string uniformName, glm::vec3 value);

void SetUniform(Rendering::Shader& shader, std::string uniformName, glm::vec4 value);

void SimpleObjRenderMVP(tst::SimpleObj& m_obj, Rendering::Shader& shader, Rendering::Texture& texture, const glm::mat4& MVP);

void SimpleObjRenderMVP(tst::SimpleObj& m_obj, Rendering::Shader& shader, const glm::mat4& MVP);

void SimpleObjRenderMVP(tst::SimpleObj& m_obj, Rendering::Shader& shader, Rendering::Texture& texture, float glide, const glm::mat4& MVP);

void MultiMatObjRenderMVP(tst::SimpleObj& m_obj, Rendering::Texture* textures, const glm::mat4& MVP);

void ImportObjN(const std::string& filePath, std::vector<float>& verts);

class SimpleObjN
{
	public:
		GLuint VAO;
		GLuint VBO;
		GLuint EBO;
		int vertexCount;

		std::vector<float> vertices;
		std::vector<int> indexes;

		SimpleObjN(const SimpleObjN &another);

		SimpleObjN(const tst::SimpleObj &another);

		SimpleObjN(const Rendering::Batch& batch);

		SimpleObjN(std::string modelFileName);

		void RenderMVP(Rendering::Shader& shader, Rendering::Texture& texture, const glm::mat4& MVP) const;

		void Dispose();
};

class WallMesh {
	public:
		WallMesh();

		void SetVertexData(const std::vector<float>& vertices);

		void SetVertexData4(const std::vector<float>& vertices);

		void SetVertexData5_rb(const std::vector<float>& vertices);

		void SetVertexData5(const std::vector<float>& vertices);

		void Render(Rendering::Shader& shader, const glm::mat4& MVP);

		void Dispose();

		GLuint VAO{0};
		GLuint VBO{0};
		GLuint EBO{0};
		int vertexCount;
};

class LightLabel{
	public:
		LightLabel();

		void Init(int size);

		void Render(Rendering::Shader& shader, Rendering::Texture& texture, const glm::mat4& MVP);

		void SetValue(int value);

		void Dispose();
		int size;
		int texSize;

		int value = 0;
		GLuint VAO{0};
		GLuint VBO{0};
		GLuint EBO{0};
		int vertexCount;
};

class ColorfulObj{
	public:
		ColorfulObj();

		void Init();

		void Render(Rendering::Shader& shader, const glm::mat4& MVP);

		void Render(Rendering::Shader& shader, const float _time, const glm::mat4& MVP);

		void Render(Rendering::Shader& shader, const float _time, const glm::vec2 end, const glm::mat4& MVP);

		void Dispose();

		GLuint uTimeLocation = 0;
		GLuint uEndLocation = 0;

		GLuint VAO;
		GLuint VBO;
		GLuint EBO;
		int vertexCount;
};

class DynamicQuad {
	public:
		DynamicQuad(int quadCount);

		void SetVertexData(const std::vector<float>& vertices);

		void Render(Rendering::Shader& shader, const glm::mat4& MVP);

		void Dispose();

		GLuint VAO{0};
		GLuint VBO{0};
		GLuint EBO{0};
		int vertexCount;
		int size;
		const int attributeCount = 2;
};

class EditableLabel{
	public:
		EditableLabel();

		void Init(int size);

		void Render(Rendering::Shader& shader, Rendering::Texture& texture, const glm::mat4& MVP);

		void GetCharPos(char smb, int& x, int& y);

		void SetValue(std::string value);

		void Dispose();

		int size{0};
		int texSize;

		std::string value;
		GLuint VAO{0};
		GLuint VBO{0};
		GLuint EBO{0};
		int vertexCount;
};

class TextMultiLine{
	public:
		TextMultiLine(float x, float y, float offset, float scale, float layer);

		TextMultiLine(float x, float y, float offset, float scale);

		void Write(std::vector<std::string> lines);
		void Render(Rendering::Shader& shader, Rendering::Texture& texture, const glm::mat4& projView);
		void SetXY(float x, float y);

		float offset;
		std::vector<EditableLabel> labels;
		glm::mat4 modelMatrix = {
			0.03, 0, 0, 0,
			0, 0.03, 0, 0,
			0, 0, 0.03, 0,
			-0.9, -0.6, 0.3, 1
		};
};

class DataGridView{
public:
	DataGridView();

	DataGridView(int n, int m);

	DataGridView(int n, int m, float labelWidth, float labelHeight);

	void BackSpace();

	void Minus();

	void AddDigit(int digit);

	template <typename T>
	void TrySelect(T point){
		auto nP = point - T(modelMatrix[3][0], modelMatrix[3][1]);
		nP = nP - T(-labelWidth * 0.25, labelHeight * 0.5);
		nP.y = -nP.y;
		if (nP.x < 0 || nP.y < 0){
			Deselect();
			return;
		}
		int ix = nP.x / (labelWidth + offsetX);
		int iy = nP.y / (labelHeight + offsetY);
		if (n == 0 || ix >= n || iy >= m) {
			Deselect();
			return;
		}
		T cCenter = T(modelMatrix[3][0] + ((offsetX + labelWidth) * ix), 
				modelMatrix[3][1] - ((offsetY + labelHeight) * iy));
		if (in_rect(point, 
			cCenter, 
			labelWidth, labelHeight))
		{
			selectionX = ix;
			selectionY = iy;
		} else {
			Deselect();
		}
	}

	void Deselect();

	void Render(Rendering::Shader& shader, Rendering::Texture& texture, const glm::mat4& projView);

	template <typename T>
	void SetPosition(T position){
		this->modelMatrix[3][0] = position.x;
		this->modelMatrix[3][1] = position.y;
	}

	float offsetX{0.01};
	float offsetY{0.01};
	float labelWidth{0.08};
	float labelHeight{0.04};
	int n{4};
	int m{4};
	std::vector<std::vector<std::string>> values;
	glm::mat4 modelMatrix = {
		0.02, 0, 0, 0,
		0, 0.02, 0, 0,
		0, 0, 0.02, 0,
		0.0, 0.0, 0.3, 1
	};
	EditableLabel* labelModel;
private:
	int selectionX{-1};
	int selectionY{-1};

	template <typename T>
	bool in_rect(T point, T center, float width, float height){
		T p1 = center + T(-width * 0.25f, -height * 0.5f);
		T p2 = center + T(width * 1.75f, -height * 0.5f);
		T p3 = center + T(-width * 0.25f, height * 0.5f);
		
		T t1 = point - p1;
		T t2 = p2 - p1;
		T t3 = point - p2;

		T t4 = p3 - p1;
		T t5 = point - p3;

		return !(T::dot(t1, t2) < 0 
			|| T::dot(t3, t2) > 0
			|| T::dot(t1, t4) < 0
			|| T::dot(t5, t4) > 0);
	}
};

class ObjectXYZUV
{
	public:
		GLuint VAO;
		GLuint VBO;
		GLuint EBO;
		int vertexCount;
		ObjectXYZUV();
		ObjectXYZUV(const std::vector<float>& mesh);
		void ChangeMesh(const std::vector<float>& mesh);
		ObjectXYZUV(const ObjectXYZUV &another);
		ObjectXYZUV(const tst::SimpleObj &another);
		ObjectXYZUV(const Rendering::Batch& batch);
		ObjectXYZUV(std::string modelFileName);
		void RenderMVP(Rendering::Shader& shader, Rendering::Texture& texture, const glm::mat4& MVP) const;
		template<typename Func>
		void RenderMVP(Rendering::Shader& shader, Rendering::Texture& texture, const glm::mat4& MVP, Func func) const{
			shader.Use();
			glUniformMatrix4fv(glGetUniformLocation(shader.programId, "MVP"), 1, GL_FALSE, &MVP[0][0]);

			func(shader);

			texture.Use();
			glBindVertexArray(this->VAO);
			glDrawElements(GL_TRIANGLES, vertexCount, GL_UNSIGNED_INT, nullptr);
			glBindVertexArray(0);
		}
		void Dispose();
};

class ObjectXYZUVS
{
	public:
		GLuint VAO;
		GLuint VBO;
		GLuint EBO;
		int vertexCount;
		ObjectXYZUVS();
		ObjectXYZUVS(const std::vector<float>& mesh);
		void ChangeMesh(const std::vector<float>& mesh);
		ObjectXYZUVS(const ObjectXYZUVS &another);
		ObjectXYZUVS(const tst::SimpleObj &another);
		ObjectXYZUVS(const Rendering::Batch& batch);
		ObjectXYZUVS(std::string modelFileName);
		void RenderMVP(Rendering::Shader& shader, Rendering::Texture& texture, const glm::mat4& MVP) const;
		//void PrepareShader(Rendering::Shader& shader, Rendering::Texture& texture, const glm::mat4& MVP) const;
		void Render() const;
		template<typename Func>
		void RenderMVP(Rendering::Shader& shader, Rendering::Texture& texture, const glm::mat4& MVP, Func func) const{
			shader.Use();
			glUniformMatrix4fv(glGetUniformLocation(shader.programId, "MVP"), 1, GL_FALSE, &MVP[0][0]);

			func(shader);

			texture.Use();
			glBindVertexArray(this->VAO);
			glDrawElements(GL_TRIANGLES, vertexCount, GL_UNSIGNED_INT, nullptr);
			glBindVertexArray(0);
		}
		void Dispose();
		static void PrepareShader(Rendering::Shader& shader, Rendering::Texture& texture, const glm::mat4& MVP);
};

class DynamicQuadXYZ {
	public:
		DynamicQuadXYZ(int quadCount);
		void SetVertexData(const std::vector<float>& vertices);
		void Render(Rendering::Shader& shader, const glm::mat4& MVP);
		void RenderOnTop(Rendering::Shader& shader, const glm::mat4& MVP);
		void Dispose();

		GLuint VAO{0};
		GLuint VBO{0};
		GLuint EBO{0};
		int vertexCount;
		int size;
		const int attributeCount = 3;
};

#endif //MY_GRAPHICS_EXTENSIONS_H