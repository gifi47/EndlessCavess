#include "MyGraphicsExtensions.h"

void SimpleObjRenderMVP(tst::SimpleObj& m_obj, Rendering::Texture& texture, const glm::mat4& MVP){
	m_obj.shader.Use();
	glUniformMatrix4fv(glGetUniformLocation(m_obj.shader.programId, "MVP"), 1, GL_FALSE, &MVP[0][0]);
	texture.Use(0);
	glBindVertexArray(m_obj.VAO);
	glDrawElements(GL_TRIANGLES, m_obj.vertexCount, GL_UNSIGNED_INT, nullptr);
	glBindVertexArray(0);
}

void SimpleObjRenderMVP(tst::SimpleObj& m_obj, Rendering::Texture& texture, float glide, const glm::mat4& MVP){
	m_obj.shader.Use();
	glUniformMatrix4fv(glGetUniformLocation(m_obj.shader.programId, "MVP"), 1, GL_FALSE, &MVP[0][0]);
	glUniform1f(glGetUniformLocation(m_obj.shader.programId, "uColor"), glide);
	texture.Use(0);
	glBindVertexArray(m_obj.VAO);
	glDrawElements(GL_TRIANGLES, m_obj.vertexCount, GL_UNSIGNED_INT, nullptr);
	glBindVertexArray(0);
}

void SetUniform(Rendering::Shader& shader, std::string uniformName, float value){
	shader.Use();
	glUniform1f(glGetUniformLocation(shader.programId, uniformName.c_str()), value);
}

void SetUniform(Rendering::Shader& shader, std::string uniformName, glm::vec3 value){
	shader.Use();
	glUniform3fv(glGetUniformLocation(shader.programId, uniformName.c_str()), 1, &value[0]);
}

void SetUniform(Rendering::Shader& shader, std::string uniformName, glm::vec4 value){
	shader.Use();
	glUniform4fv(glGetUniformLocation(shader.programId, uniformName.c_str()), 1, &value[0]);
}

void SimpleObjRenderMVP(tst::SimpleObj& m_obj, Rendering::Shader& shader, Rendering::Texture& texture, const glm::mat4& MVP){
	shader.Use();
	glUniformMatrix4fv(glGetUniformLocation(shader.programId, "MVP"), 1, GL_FALSE, &MVP[0][0]);
	texture.Use(0);
	glBindVertexArray(m_obj.VAO);
	glDrawElements(GL_TRIANGLES, m_obj.vertexCount, GL_UNSIGNED_INT, nullptr);
	glBindVertexArray(0);
}

void SimpleObjRenderMVP(tst::SimpleObj& m_obj, Rendering::Shader& shader, const glm::mat4& MVP){
	shader.Use();
	glUniformMatrix4fv(glGetUniformLocation(shader.programId, "MVP"), 1, GL_FALSE, &MVP[0][0]);
	glBindVertexArray(m_obj.VAO);
	glDrawElements(GL_TRIANGLES, m_obj.vertexCount, GL_UNSIGNED_INT, nullptr);
	glBindVertexArray(0);
}

void SimpleObjRenderMVP(tst::SimpleObj& m_obj, Rendering::Shader& shader, Rendering::Texture& texture, float glide, const glm::mat4& MVP){
	shader.Use();
	glUniformMatrix4fv(glGetUniformLocation(m_obj.shader.programId, "MVP"), 1, GL_FALSE, &MVP[0][0]);
	glUniform1f(glGetUniformLocation(m_obj.shader.programId, "uColor"), glide);
	texture.Use(0);
	glBindVertexArray(m_obj.VAO);
	glDrawElements(GL_TRIANGLES, m_obj.vertexCount, GL_UNSIGNED_INT, nullptr);
	glBindVertexArray(0);
}

void MultiMatObjRenderMVP(tst::SimpleObj& m_obj, Rendering::Texture* textures, const glm::mat4& MVP){
	m_obj.shader.Use();
	glUniformMatrix4fv(glGetUniformLocation(m_obj.shader.programId, "MVP"), 1, GL_FALSE, &MVP[0][0]);
	textures[0].Use(0);
	textures[1].Use(1);
	textures[2].Use(2);
	textures[3].Use(3);
	glBindVertexArray(m_obj.VAO);
	glDrawElements(GL_TRIANGLES, m_obj.vertexCount, GL_UNSIGNED_INT, nullptr);
	glBindVertexArray(0);
}

void ImportObjN(const std::string& filePath, std::vector<float>& verts){
    std::ifstream fin(filePath, std::ios::in);
    std::string tp;
    std::vector<float> positions;
    std::vector<float> texCoords;
    std::vector<float> normals;
    std::vector<int> positionsIndexes;
    while (fin >> tp) {
        if (tp.size() == 1) {
            switch (tp[0])
            {
            case 'v':
                float x, y, z;
                fin >> x >> y >> z;
                positions.push_back(x);
                positions.push_back(y);
                positions.push_back(z);

                break;
            case 'f':
                for (int i7 = 0; i7 < 3; i7++) {
                    int p1, p2, p3;
                    char temp;
                    fin >> p1 >> temp >> p2 >> temp >> p3;
                    positionsIndexes.push_back(p1 - 1);
                    positionsIndexes.push_back(p2 - 1);
                    positionsIndexes.push_back(p3 - 1);
                }
                break;
            default:
                std::string s;
                std::getline(fin, s);
                break;
            }
        }
        else {
            if (tp[0] == 'v' && tp[1] == 't') {
                float t1, t2;
                fin >> t1 >> t2;
                texCoords.push_back(t1);
                texCoords.push_back(t2);
            } else if (tp[0] == 'v' && tp[1] == 'n'){
            	float x, y, z;
                fin >> x >> y >> z;
                normals.push_back(x);
                normals.push_back(y);
                normals.push_back(z);
            }
            else {
                std::string s;
                std::getline(fin, s);
            }
        }
    }
    verts.resize(0);
    for (int i = 0; i < positionsIndexes.size(); i+=3) {
        int ind = positionsIndexes[i] * 3;
        
        int indTexCoord = positionsIndexes[i + 1] * 2;
        
        int indNormal = positionsIndexes[i + 2] * 3;

        verts.push_back(positions[ind]);
        verts.push_back(positions[ind + 1]);
        verts.push_back(positions[ind + 2]);

        verts.push_back(normals[indNormal]);
        verts.push_back(normals[indNormal + 1]);
        verts.push_back(normals[indNormal + 2]);

        verts.push_back(texCoords[indTexCoord]);
        verts.push_back(texCoords[indTexCoord + 1]);
    }
    fin.close();
    std::cout << "Loaded " << filePath << " verts loaded:" << verts.size()/8 << "\n";
}


SimpleObjN::SimpleObjN(const SimpleObjN &another){
	this->VAO = another.VAO;
	this->VBO = another.VBO;
	this->EBO = another.EBO;
	this->vertexCount = another.vertexCount;
}

SimpleObjN::SimpleObjN(const tst::SimpleObj &another){
	this->VAO = another.VAO;
	this->VBO = another.VBO;
	this->EBO = another.EBO;
	this->vertexCount = another.vertexCount;
}

SimpleObjN::SimpleObjN(const Rendering::Batch& batch){
	this->VAO = batch.VAO;
	this->VBO = batch.VBO;
	this->EBO = batch.EBO;
	this->vertexCount = batch.vertexCount;
}

SimpleObjN::SimpleObjN(std::string modelFileName){
	std::vector<float> vertices;
	ImportObjN(modelFileName, vertices);

	glGenVertexArrays(1, &this->VAO);
	glGenBuffers(1, &this->VBO);
	glGenBuffers(1, &this->EBO);
	
	int attributeCount = 8;

	std::vector<int> indexes(vertices.size() / attributeCount);
	indexes.resize(vertices.size() / attributeCount);
	for (int i = 0; i < indexes.size(); i++) {
		indexes[i] = i;
	}

	this->vertexCount = indexes.size();

	glBindVertexArray(this->VAO);

	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * indexes.size(), indexes.data(), GL_STATIC_DRAW);

	// XYZ координаты
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, attributeCount * sizeof(float), (GLvoid*)nullptr);
	glEnableVertexAttribArray(0);

	// Нормали
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, attributeCount * sizeof(float), (GLvoid*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	
	// Текстурные координаты
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, attributeCount * sizeof(float), (GLvoid*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
}

void SimpleObjN::RenderMVP(Rendering::Shader& shader, Rendering::Texture& texture, const glm::mat4& MVP) const{
	shader.Use();
	glUniformMatrix4fv(glGetUniformLocation(shader.programId, "MVP"), 1, GL_FALSE, &MVP[0][0]);
	texture.Use();
	glBindVertexArray(this->VAO);
	glDrawElements(GL_TRIANGLES, vertexCount, GL_UNSIGNED_INT, nullptr);
	glBindVertexArray(0);
}

void SimpleObjN::Dispose(){
	glBindVertexArray(this->VAO);
	glDeleteVertexArrays(1, &this->VAO);
	glBindVertexArray(0);
	glDeleteBuffers(1, &this->VBO);
	glDeleteBuffers(1, &this->EBO);
}


WallMesh::WallMesh()
{

}

void WallMesh::SetVertexData(const std::vector<float>& vertices)
{	
	if (this->VAO == 0)
		glGenVertexArrays(1, &this->VAO);
	if (this->VBO == 0)
		glGenBuffers(1, &this->VBO);
	if (this->EBO == 0)
		glGenBuffers(1, &this->EBO);
	
	int attributeCount = 2;

	std::vector<int> indexes((vertices.size() / (4 * attributeCount)) * 6);
	int k = 0;
	for (int i = 0; i < indexes.size(); i+=6) {
		indexes[i] = k;
		indexes[i + 1] = k + 2;
		indexes[i + 2] = k + 1;
		indexes[i + 3] = k + 2;
		indexes[i + 4] = k;
		indexes[i + 5] = k + 3;
		k += 4;
	}

	this->vertexCount = indexes.size();

	glBindVertexArray(this->VAO);

	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * indexes.size(), indexes.data(), GL_STATIC_DRAW);

	// XY COORDS
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, attributeCount * sizeof(float), (GLvoid*)nullptr);
	glEnableVertexAttribArray(0);

	glDisableVertexAttribArray(1);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
}

void WallMesh::SetVertexData4(const std::vector<float>& vertices)
{	
	if (this->VAO == 0)
		glGenVertexArrays(1, &this->VAO);
	if (this->VBO == 0)
		glGenBuffers(1, &this->VBO);
	if (this->EBO == 0)
		glGenBuffers(1, &this->EBO);
	
	int attributeCount = 4;

	std::vector<int> indexes((vertices.size() / (4 * attributeCount)) * 6);
	int k = 0;
	for (int i = 0; i < indexes.size(); i+=6) {
		indexes[i] = k;
		indexes[i + 1] = k + 2;
		indexes[i + 2] = k + 1;
		indexes[i + 3] = k + 2;
		indexes[i + 4] = k;
		indexes[i + 5] = k + 3;
		k += 4;
	}

	this->vertexCount = indexes.size();

	glBindVertexArray(this->VAO);

	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * indexes.size(), indexes.data(), GL_STATIC_DRAW);

	// XY COORDS
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, attributeCount * sizeof(float), (GLvoid*)nullptr);
	glEnableVertexAttribArray(0);

	// ST COORDS
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, attributeCount * sizeof(float), (GLvoid*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
}

void WallMesh::SetVertexData5_rb(const std::vector<float>& vertices)
{	
	if (this->VAO == 0)
		glGenVertexArrays(1, &this->VAO);
	if (this->VBO == 0)
		glGenBuffers(1, &this->VBO);
	if (this->EBO == 0)
		glGenBuffers(1, &this->EBO);
	
	int attributeCount = 5;

	std::vector<int> indexes((vertices.size() / (4 * attributeCount)) * 6);
	int k = 0;
	for (int i = 0; i < indexes.size(); i+=6) {
		indexes[i] = k;
		indexes[i + 1] = k + 2;
		indexes[i + 2] = k + 1;
		indexes[i + 3] = k + 2;
		indexes[i + 4] = k;
		indexes[i + 5] = k + 3;
		k += 4;
	}

	this->vertexCount = indexes.size();

	glBindVertexArray(this->VAO);

	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * indexes.size(), indexes.data(), GL_STATIC_DRAW);

	// XY COORDS
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, attributeCount * sizeof(float), (GLvoid*)nullptr);
	glEnableVertexAttribArray(0);

	// ST COORDS
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, attributeCount * sizeof(float), (GLvoid*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// R Color
	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, attributeCount * sizeof(float), (GLvoid*)(4 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
}

void WallMesh::SetVertexData5(const std::vector<float>& vertices)
{	
	if (this->VAO == 0)
		glGenVertexArrays(1, &this->VAO);
	if (this->VBO == 0)
		glGenBuffers(1, &this->VBO);
	if (this->EBO == 0)
		glGenBuffers(1, &this->EBO);
	
	int attributeCount = 5;

	std::vector<int> indexes((vertices.size() / (4 * attributeCount)) * 6);
	int k = 0;
	for (int i = 0; i < indexes.size(); i+=6) {
		indexes[i] = k;
		indexes[i + 1] = k + 2;
		indexes[i + 2] = k + 1;
		indexes[i + 3] = k + 2;
		indexes[i + 4] = k;
		indexes[i + 5] = k + 3;
		k += 4;
	}

	this->vertexCount = indexes.size();

	glBindVertexArray(this->VAO);

	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * indexes.size(), indexes.data(), GL_STATIC_DRAW);

	// XY COORDS
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, attributeCount * sizeof(float), (GLvoid*)nullptr);
	glEnableVertexAttribArray(0);

	// RGB COLORS
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, attributeCount * sizeof(float), (GLvoid*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
}

void WallMesh::Render(Rendering::Shader& shader, const glm::mat4& MVP)
{
	shader.Use();
	glUniformMatrix4fv(glGetUniformLocation(shader.programId, "MVP"), 1, GL_FALSE, &MVP[0][0]);
	glBindVertexArray(this->VAO);
	glDrawElements(GL_TRIANGLES, this->vertexCount, GL_UNSIGNED_INT, nullptr);
	glBindVertexArray(0);
}

void WallMesh::Dispose()
{
	glBindVertexArray(this->VAO);
	glDeleteVertexArrays(1, &this->VAO);
	glBindVertexArray(0);
	glDeleteBuffers(1, &this->VBO);
	glDeleteBuffers(1, &this->EBO);
}


LightLabel::LightLabel() {
	
}

void LightLabel::Init(int size){
	this->size = size;

	std::vector<float> verts(size * 8);
	int k = 0;
	for (int i = 0; i < verts.size(); i += 8) {
		verts[i] = -1 + k; verts[i + 1] = 1;
		verts[i + 2] = -1 + k; verts[i + 3] = -1;
		verts[i + 4] = 1 + k; verts[i + 5] = -1;
		verts[i + 6] = 1 + k; verts[i + 7] = 1;
		k += 2;
	}
	this->texSize = size * 8;

	std::vector<float> tex(size * 8);
	for (int i = 0; i < tex.size(); i += 8) {
		tex[i] = 0; tex[i + 1] = 1;
		tex[i + 2] = 0; tex[i + 3] = 0;
		tex[i + 4] = 0.1; tex[i + 5] = 0;
		tex[i + 6] = 0.1; tex[i + 7] = 1;
	}

	std::vector<int> indexes(size * 6);
	k = 0;
	for (int i = 0; i < indexes.size(); i+=6) {
		indexes[i] = 0 + k;
		indexes[i + 1] = 1 + k;
		indexes[i + 2] = 2 + k;
		indexes[i + 3] = 2 + k;
		indexes[i + 4] = 3 + k;
		indexes[i + 5] = 0 + k;
		k += 4;
	}
	glGenVertexArrays(1, &this->VAO);
	glGenBuffers(1, &this->VBO);
	glGenBuffers(1, &this->EBO);
	this->vertexCount = indexes.size();

	glBindVertexArray(this->VAO);

	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * verts.size() * tex.size(), nullptr, GL_DYNAMIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * verts.size(), verts.data());
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(float) * verts.size(), sizeof(float) * tex.size(), tex.data());

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * indexes.size(), indexes.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (GLvoid*)nullptr);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (GLvoid*)(verts.size() * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void LightLabel::Render(Rendering::Shader& shader, Rendering::Texture& texture, const glm::mat4& MVP) {
	shader.Use();
	glUniformMatrix4fv(glGetUniformLocation(shader.programId, "MVP"), 1, GL_FALSE, &MVP[0][0]);
	texture.Use();
	glBindVertexArray(this->VAO);
	glDrawElements(GL_TRIANGLES, vertexCount, GL_UNSIGNED_INT, nullptr);
	glBindVertexArray(0);
}

void LightLabel::SetValue(int value) {
	if (value == this->value) return;
	this->value = value;

	std::vector<float> tex(texSize);

	std::string str;
	if (value == 0) str = "-0";
	else if (value < 0) str = "--";
	else 
	{
		if (value < 10) value = -value;
		str = std::to_string(value);
	}
	int k = str.size() - size;
	for (int i = 0; i < tex.size(); i += 8) {
		float left = (k < 0 ? 0 : (str[k] - '0') * 0.1);
		k++;
		tex[i] = left; tex[i + 1] = 1;
		tex[i + 2] = left; tex[i + 3] = 0;
		tex[i + 4] = left + 0.1; tex[i + 5] = 0;
		tex[i + 6] = left + 0.1; tex[i + 7] = 1;
	}

	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(float) * size * 8, sizeof(float) * tex.size(), tex.data());

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void LightLabel::Dispose() {
	glBindVertexArray(this->VAO);
	glDeleteVertexArrays(1, &this->VAO);
	glBindVertexArray(0);
	glDeleteBuffers(1, &this->VBO);
	glDeleteBuffers(1, &this->EBO);
}

ColorfulObj::ColorfulObj() {
	
}

void ColorfulObj::Init()
{	
	glGenVertexArrays(1, &this->VAO);
	glGenBuffers(1, &this->VBO);
	glGenBuffers(1, &this->EBO);
	
	int attributeCount = 4;

	std::vector<float> vertices(4 * attributeCount);

	int _x = 1;
	int _y = 1;
	for (int i = 0; i < 4; i++){
		vertices[i * attributeCount] = _x; vertices[i * attributeCount + 1] = _y;
		vertices[i * attributeCount + 2] = (_x < 0 ? 0 : _x); vertices[i * attributeCount + 3] = (_y < 0 ? 0 : _y);
		if (_x < 0) _y = -1;
		_x = -_x;
	}

	std::vector<int> indexes((vertices.size() / (4 * attributeCount)) * 6);
	int k = 0;
	for (int i = 0; i < indexes.size(); i+=6) {
		indexes[i] = k + 1;
		indexes[i + 1] = k + 3;
		indexes[i + 2] = k + 0;
		indexes[i + 3] = k + 0;
		indexes[i + 4] = k + 3;
		indexes[i + 5] = k + 2;
		k += 4;
	}

	this->vertexCount = indexes.size();

	glBindVertexArray(this->VAO);

	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * indexes.size(), indexes.data(), GL_STATIC_DRAW);

	// XY COORDS
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, attributeCount * sizeof(float), (GLvoid*)nullptr);
	glEnableVertexAttribArray(0);

	// UV COORDS
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, attributeCount * sizeof(float), (GLvoid*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
}

void ColorfulObj::Render(Rendering::Shader& shader, const glm::mat4& MVP)
{
	shader.Use();
	glUniformMatrix4fv(glGetUniformLocation(shader.programId, "MVP"), 1, GL_FALSE, &MVP[0][0]);
	glBindVertexArray(this->VAO);
	glDrawElements(GL_TRIANGLES, this->vertexCount, GL_UNSIGNED_INT, nullptr);
	glBindVertexArray(0);
}

void ColorfulObj::Render(Rendering::Shader& shader, const float _time, const glm::mat4& MVP)
{
	shader.Use();
	glUniformMatrix4fv(glGetUniformLocation(shader.programId, "MVP"), 1, GL_FALSE, &MVP[0][0]);
	glUniform1f(glGetUniformLocation(shader.programId, "time"), _time);
	glBindVertexArray(this->VAO);
	glDrawElements(GL_TRIANGLES, this->vertexCount, GL_UNSIGNED_INT, nullptr);
	glBindVertexArray(0);
}

void ColorfulObj::Render(Rendering::Shader& shader, const float _time, const glm::vec2 end, const glm::mat4& MVP)
{
	shader.Use();
	glUniformMatrix4fv(glGetUniformLocation(shader.programId, "MVP"), 1, GL_FALSE, &MVP[0][0]);
	
	if (uTimeLocation == 0)
		uTimeLocation = glGetUniformLocation(shader.programId, "time");
	glUniform1f(uTimeLocation, _time);
	
	if (uEndLocation == 0)
		uEndLocation = glGetUniformLocation(shader.programId, "end");
	glUniform2f(uEndLocation, end.x, end.y);
	
	glBindVertexArray(this->VAO);
	glDrawElements(GL_TRIANGLES, this->vertexCount, GL_UNSIGNED_INT, nullptr);
	glBindVertexArray(0);
}

void ColorfulObj::Dispose()
{
	glBindVertexArray(this->VAO);
	glDeleteVertexArrays(1, &this->VAO);
	glBindVertexArray(0);
	glDeleteBuffers(1, &this->VBO);
	glDeleteBuffers(1, &this->EBO);
}

DynamicQuad::DynamicQuad(int quadCount)
{
	this->size = quadCount;

	glGenVertexArrays(1, &this->VAO);
	glGenBuffers(1, &this->VBO);
	glGenBuffers(1, &this->EBO);

	std::vector<int> indexes(6 * this->size);
	int k = 0;
	for (int i = 0; i < indexes.size(); i+=6) {
		indexes[i] = k;
		indexes[i + 1] = k + 2;
		indexes[i + 2] = k + 1;
		indexes[i + 3] = k + 2;
		indexes[i + 4] = k;
		indexes[i + 5] = k + 3;
		k += 4;
	}

	this->vertexCount = indexes.size();

	glBindVertexArray(this->VAO);

	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 4 * attributeCount * size, nullptr, GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * indexes.size(), indexes.data(), GL_STATIC_DRAW);

	// XY COORDS
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, attributeCount * sizeof(float), (GLvoid*)nullptr);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
}

void DynamicQuad::SetVertexData(const std::vector<float>& vertices)
{	
	if (vertices.size() != attributeCount * 4 * size)
		throw "Incorrect Size of a vertices array!";

	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * vertices.size(), vertices.data());

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void DynamicQuad::Render(Rendering::Shader& shader, const glm::mat4& MVP)
{
	shader.Use();
	glUniformMatrix4fv(glGetUniformLocation(shader.programId, "MVP"), 1, GL_FALSE, &MVP[0][0]);
	glBindVertexArray(this->VAO);
	glDrawElements(GL_TRIANGLES, this->vertexCount, GL_UNSIGNED_INT, nullptr);
	glBindVertexArray(0);
}

void DynamicQuad::Dispose()
{
	glBindVertexArray(this->VAO);
	glDeleteVertexArrays(1, &this->VAO);
	glBindVertexArray(0);
	glDeleteBuffers(1, &this->VBO);
	glDeleteBuffers(1, &this->EBO);
}

EditableLabel::EditableLabel() {
	
}

void EditableLabel::Init(int size){
	this->size = size;

	std::vector<float> verts(size * 8);
	int k = 0;
	for (int i = 0; i < verts.size(); i += 8) {
		verts[i] = -1 + k; verts[i + 1] = 1;
		verts[i + 2] = -1 + k; verts[i + 3] = -1;
		verts[i + 4] = 1 + k; verts[i + 5] = -1;
		verts[i + 6] = 1 + k; verts[i + 7] = 1;
		k += 2;
	}
	this->texSize = size * 8;

	std::vector<float> tex(size * 8);
	for (int i = 0; i < tex.size(); i += 8) {
		tex[i] = 0; tex[i + 1] = 1;
		tex[i + 2] = 0; tex[i + 3] = 0.9;
		tex[i + 4] = 0.1; tex[i + 5] = 0.9;
		tex[i + 6] = 0.1; tex[i + 7] = 1;
	}

	std::vector<int> indexes(size * 6);
	k = 0;
	for (int i = 0; i < indexes.size(); i+=6) {
		indexes[i] = 0 + k;
		indexes[i + 1] = 1 + k;
		indexes[i + 2] = 2 + k;
		indexes[i + 3] = 2 + k;
		indexes[i + 4] = 3 + k;
		indexes[i + 5] = 0 + k;
		k += 4;
	}
	if (this->VAO == 0)
		glGenVertexArrays(1, &this->VAO);
	if (this->VBO == 0)
		glGenBuffers(1, &this->VBO);
	if (this->EBO == 0)
		glGenBuffers(1, &this->EBO);
	
	this->vertexCount = indexes.size();

	glBindVertexArray(this->VAO);

	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * verts.size() * tex.size(), nullptr, GL_DYNAMIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * verts.size(), verts.data());
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(float) * verts.size(), sizeof(float) * tex.size(), tex.data());

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * indexes.size(), indexes.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (GLvoid*)nullptr);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (GLvoid*)(verts.size() * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void EditableLabel::Render(Rendering::Shader& shader, Rendering::Texture& texture, const glm::mat4& MVP) {
	shader.Use();
	glUniformMatrix4fv(glGetUniformLocation(shader.programId, "MVP"), 1, GL_FALSE, &MVP[0][0]);
	texture.Use();
	glBindVertexArray(this->VAO);
	glDrawElements(GL_TRIANGLES, vertexCount, GL_UNSIGNED_INT, nullptr);
	glBindVertexArray(0);
}

void EditableLabel::GetCharPos(char smb, int& x, int& y) {
	if ('0' <= smb && smb <= '3'){
		x = (int)(smb - '0') + 6;
		y = 9;
		return;
	}
	if ('4' <= smb && smb <= '9'){
		x = (int)(smb - '4');
		y = 8;
		return;
	}
	if ('a' <= smb && smb <= 'j')
	{
		x = (int)(smb - 'a');
		y = 3;
		return;
	}
	if ('k' <= smb && smb <= 't')
	{
		x = (int)(smb - 'k');
		y = 2;
		return;
	}
	if ('u' <= smb && smb <= 'z')
	{
		x = (int)(smb - 'u');
		y = 1;
		return;
	}
	if ('A' <= smb && smb <= 'J')
	{
		x = (int)(smb - 'A');
		y = 3;
		return;
	}
	if ('K' <= smb && smb <= 'T')
	{
		x = (int)(smb - 'K');
		y = 2;
		return;
	}
	if ('U' <= smb && smb <= 'Z')
	{
		x = (int)(smb - 'U');
		y = 1;
		return;
	}
	if (192 <= ((unsigned char)smb) && ((unsigned char)smb) <= 195)
	{
		x = (((int)((unsigned char)smb)) - 192) + 6;
		y = 8;
		return;
	}
	if (198 <= ((unsigned char)smb) && ((unsigned char)smb) <= 204)
	{
		x = (((int)((unsigned char)smb)) - 198) + 3;
		y = 7;
		return;
	}
	if (205 <= ((unsigned char)smb) && ((unsigned char)smb) <= 214)
	{
		x = (((int)((unsigned char)smb)) - 205);
		y = 6;
		return;
	}
	if (215 <= ((unsigned char)smb) && ((unsigned char)smb) <= 223)
	{
		x = (((int)((unsigned char)smb)) - 215);
		y = 5;
		return;
	}
	switch (smb){
		case ' ':
			x = 0;
			y = 9;
			return;
		case '-':
			x = 1;
			y = 9;
			return;
		case '+':
			x = 2;
			y = 9;
			return;
		case ':':
			x = 3;
			y = 9;
			return;
		case '(':
			x = 4;
			y = 9;
			return;
		case ')':
			x = 5;
			y = 9;
			return;
		case '.':
			x = 9;
			y = 5;
			return;
		case ',':
			x = 0;
			y = 4;
			return;
		case '[':
			x = 1;
			y = 4;
			return;
		case ']':
			x = 2;
			y = 4;
			return;
		case '@': // CARET
			x = 3;
			y = 4;
			return;
		case '~': // BLANK
			x = 4;
			y = 4;
			return;
		case '<':
			x = 5;
			y = 4;
			return;
		case '>':
			x = 6;
			y = 4;
			return;
		case '=':
			x = 7;
			y = 4;
			return;
		case '_':
			x = 8;
			y = 4;
			return;
		case '?':
			x = 9;
			y = 4;
			return;
		case '*':
			x = 6;
			y = 1;
			return;
		case '/':
			x = 7;
			y = 1;
			return;
		case '{': // CENTER DOT
			x = 8;
			y = 1;
			return;
		case '}': // LONG MINUS
			x = 9;
			y = 1;
			return;
		case '#':
			x = 0;
			y = 0;
			return;
		case '%':
			x = 1;
			y = 0;
			return;
		case '$':
			x = 2;
			y = 0;
			return;
		case '^': // RUB SMB
			x = 3;
			y = 0;
			return;
		case '!':
			x = 4;
			y = 0;
			return;
		case ((char)(185)): //№ N
			x = 5;
			y = 0;
			return;
		case ';':
			x = 6;
			y = 0;
			return;
		case '\'':
			x = 7;
			y = 0;
			return;
		case '\"':
			x = 8;
			y = 0;
			return;
		case '|':
			x = 9;
			y = 0;
			return;
		case ((char)(196)): // д
			x = 0;
			y = 7;
			return;
		case ((char)(197)): // e
			x = 1;
			y = 7;
			return;
		case ((char)(168)): // ё
			x = 2;
			y = 7;
			return;
	}
	x = 9;
	y = 4;
	return;
}

void EditableLabel::SetValue(std::string value) {
	if (value == this->value) return;
	this->value = value;

	std::vector<float> tex(texSize);

	int k = value.size() - size;
	for (int i = 0; i < tex.size(); i += 8) {
		int x = 0;
		int y = 0;
		GetCharPos(k < 0 ? ' ' : value[k], x, y);
		float left = x * 0.1;
		float up = y * 0.1;
		k++;
		tex[i] = left; 				tex[i + 1] = up + 0.1;
		tex[i + 2] = left; 			tex[i + 3] = up;
		tex[i + 4] = left + 0.1; 	tex[i + 5] = up;
		tex[i + 6] = left + 0.1; 	tex[i + 7] = up + 0.1;
	}

	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(float) * size * 8, sizeof(float) * tex.size(), tex.data());

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void EditableLabel::Dispose() {
	glBindVertexArray(this->VAO);
	glDeleteVertexArrays(1, &this->VAO);
	glBindVertexArray(0);
	glDeleteBuffers(1, &this->VBO);
	glDeleteBuffers(1, &this->EBO);
}

TextMultiLine::TextMultiLine(float x, float y, float offset, float scale, float layer)
{
	modelMatrix[0][0] = scale;
	modelMatrix[1][1] = scale;
	modelMatrix[2][2] = scale;
	modelMatrix[3][0] = x;
	modelMatrix[3][1] = y;
	modelMatrix[3][2] = layer;
	this->offset = offset;
}

TextMultiLine::TextMultiLine(float x, float y, float offset, float scale)
{
	modelMatrix[0][0] = scale;
	modelMatrix[1][1] = scale;
	modelMatrix[2][2] = scale;
	modelMatrix[3][0] = x;
	modelMatrix[3][1] = y;
	this->offset = offset;
}

void TextMultiLine::Write(std::vector<std::string> lines)
{
	for (int i = 0; i < lines.size(); i++){
		if (labels.size() == i){
			labels.push_back(EditableLabel());
		}
		if (labels[i].size != lines[i].size()){
			labels[i].Init(lines[i].size());
		}
		std::cout << lines[i] << "\n";
		labels[i].SetValue(lines[i]);
	}
}

void TextMultiLine::Render(Rendering::Shader& shader, Rendering::Texture& texture, const glm::mat4& projView)
{
	for (int i = 0; i < labels.size(); i++){
		glm::mat4 localModelMat = modelMatrix;
		localModelMat[3][1] -= offset * i;
		localModelMat = projView * localModelMat;
		labels[i].Render(shader, texture, localModelMat);
	}
}

void TextMultiLine::SetXY(float x, float y)
{
	modelMatrix[3][0] = x;
	modelMatrix[3][1] = y;
}

DataGridView::DataGridView()
{
	values = std::vector<std::vector<std::string>>(n, std::vector<std::string>(m, "0"));
}

DataGridView::DataGridView(int n, int m) : n{n}, m{m}
{
	values = std::vector<std::vector<std::string>>(n, std::vector<std::string>(m, "0"));
}

DataGridView::DataGridView(int n, int m, float labelWidth, float labelHeight) : n{n}, m{m}, labelWidth{labelWidth}, labelHeight{labelHeight}
{
	modelMatrix[0][0] = labelHeight * 0.5f;
	modelMatrix[1][1] = labelHeight * 0.5f;
	modelMatrix[2][2] = labelHeight * 0.5f;
	values = std::vector<std::vector<std::string>>(n, std::vector<std::string>(m, "0"));
}

void DataGridView::BackSpace()
{
	if (selectionX != -1 && selectionY != -1){
		if (values[selectionX][selectionY][0] == '-'){
			if (values[selectionX][selectionY].size() == 1){
				values[selectionX][selectionY] = "0";
				return;
			}
			else
				values[selectionX][selectionY].pop_back();
			if (values[selectionX][selectionY][1] == '0') 
				values[selectionX][selectionY].pop_back();
		}
		else {
			if (values[selectionX][selectionY].size() == 1)
				values[selectionX][selectionY] = "0";
			else
				values[selectionX][selectionY].pop_back();
		}
	}
}

void DataGridView::Minus(){
	if (selectionX != -1 && selectionY != -1){
		if (values[selectionX][selectionY][0] == '-'){
			values[selectionX][selectionY].erase(0, 1);
			if (values[selectionX][selectionY].size() == 0) values[selectionX][selectionY] = "0";
		} else {
			if (values[selectionX][selectionY][0] == '0')
				values[selectionX][selectionY] = "-";
			else
				values[selectionX][selectionY] = "-" + values[selectionX][selectionY];
		}
	}
}

void DataGridView::AddDigit(int digit)
{
	if (selectionX != -1 && selectionY != -1){
		if (values[selectionX][selectionY][0] == '-'){
			if (values[selectionX][selectionY].size() > 3) return;
			if (values[selectionX][selectionY].size() == 1 && digit == 0) return;
			values[selectionX][selectionY].push_back(((char)digit) + '0');
		} else {
			if (values[selectionX][selectionY].size() > 2) return;
			if (values[selectionX][selectionY][0] == '0') {
				if (digit == 0) return;
				values[selectionX][selectionY][0] = (((char)digit) + '0');
				return;
			}
			values[selectionX][selectionY].push_back(((char)digit) + '0');
		}
	}
}

/*template <typename T>
void DataGridView::TrySelect(T point)
{
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
}*/

void DataGridView::Deselect(){
	if (selectionX != -1 && selectionY != -1){
		if (values[selectionX][selectionY] == "-") values[selectionX][selectionY] = "0";
	}
	selectionX = -1;
	selectionY = -1;
}

void DataGridView::Render(Rendering::Shader& shader, Rendering::Texture& texture, const glm::mat4& projView){
	for (int i = 0; i < n; i++){
		for (int j = 0; j < m; j++){

			glm::mat4 localModelMat = modelMatrix;
			localModelMat[3][0] += (offsetX + labelWidth) * i;
			localModelMat[3][1] -= (offsetY + labelHeight) * j;
			localModelMat = projView * localModelMat;
			if (labelModel->size != 4) labelModel->Init(4);
			//if (labelModel->size != 2) labelModel->Init(2);

			labelModel->SetValue(values[i][j]);

			if (selectionX == i && selectionY == j){
				SetUniform(shader, "uHighLightColor", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
				//SetUniform(shader, "uColor", glm::vec3(0.0f, 0.0f, 0.0f));
			}
			labelModel->Render(shader, texture, projView * localModelMat);
			if (selectionX == i && selectionY == j){
				SetUniform(shader, "uHighLightColor", glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
				//SetUniform(shader, "uColor", glm::vec3(1.0f, 1.0f, 1.0f));
			}
		}
	}
}

/*template <typename T>
void DataGridView::SetPosition(T position){
	this->modelMatrix[3][0] = position.x;
	this->modelMatrix[3][1] = position.y;
}*/

/*template <typename T>
bool DataGridView::in_rect(T point, T center, float width, float height){
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
}*/

ObjectXYZUV::ObjectXYZUV(){}
ObjectXYZUV::ObjectXYZUV(const std::vector<float>& mesh){
	glGenVertexArrays(1, &this->VAO);
	glGenBuffers(1, &this->VBO);
	glGenBuffers(1, &this->EBO);
	
	int attributeCount = 5;
	std::vector<int> indexes;
	indexes.resize(mesh.size() / attributeCount / 4 * 6);
	for (int i = 0, k = 0; i < indexes.size(); i+=6, k+=4) {
		indexes[i + 0] = k + 0;
		indexes[i + 1] = k + 1;
		indexes[i + 2] = k + 2;
		indexes[i + 3] = k + 0;
		indexes[i + 4] = k + 2;
		indexes[i + 5] = k + 3;
	}

	this->vertexCount = indexes.size();

	glBindVertexArray(this->VAO);

	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mesh.size(), mesh.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * indexes.size(), indexes.data(), GL_STATIC_DRAW);

	// XYZ координаты
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, attributeCount * sizeof(float), (GLvoid*)nullptr);
	glEnableVertexAttribArray(0);
	
	// UV Текстурные координаты
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, attributeCount * sizeof(float), (GLvoid*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
}

void ObjectXYZUV::ChangeMesh(const std::vector<float>& mesh){
	int attributeCount = 5;
	std::vector<int> indexes;
	indexes.resize(mesh.size() / attributeCount / 4 * 6);
	for (int i = 0, k = 0; i < indexes.size(); i+=6, k+=4) {
		indexes[i + 0] = k + 0;
		indexes[i + 1] = k + 1;
		indexes[i + 2] = k + 2;
		indexes[i + 3] = k + 0;
		indexes[i + 4] = k + 2;
		indexes[i + 5] = k + 3;
	}

	this->vertexCount = indexes.size();

	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mesh.size(), mesh.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * indexes.size(), indexes.data(), GL_STATIC_DRAW);
}

ObjectXYZUV::ObjectXYZUV(const ObjectXYZUV &another){
	this->VAO = another.VAO;
	this->VBO = another.VBO;
	this->EBO = another.EBO;
	this->vertexCount = another.vertexCount;
}

ObjectXYZUV::ObjectXYZUV(const tst::SimpleObj &another){
	this->VAO = another.VAO;
	this->VBO = another.VBO;
	this->EBO = another.EBO;
	this->vertexCount = another.vertexCount;
}

ObjectXYZUV::ObjectXYZUV(const Rendering::Batch& batch){
	this->VAO = batch.VAO;
	this->VBO = batch.VBO;
	this->EBO = batch.EBO;
	this->vertexCount = batch.vertexCount;
}

ObjectXYZUV::ObjectXYZUV(std::string modelFileName){
	std::vector<float> vertices;
	ImportObjN(modelFileName, vertices);

	glGenVertexArrays(1, &this->VAO);
	glGenBuffers(1, &this->VBO);
	glGenBuffers(1, &this->EBO);
	
	int attributeCount = 5;

	std::vector<int> indexes;
	indexes.resize(vertices.size() / attributeCount);
	for (int i = 0; i < indexes.size(); i++) {
		indexes[i] = i;
	}

	this->vertexCount = indexes.size();

	glBindVertexArray(this->VAO);

	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * indexes.size(), indexes.data(), GL_STATIC_DRAW);

	// XYZ координаты
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, attributeCount * sizeof(float), (GLvoid*)nullptr);
	glEnableVertexAttribArray(0);
	
	// UV Текстурные координаты
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, attributeCount * sizeof(float), (GLvoid*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
}

void ObjectXYZUV::RenderMVP(Rendering::Shader& shader, Rendering::Texture& texture, const glm::mat4& MVP) const{
	shader.Use();
	glUniformMatrix4fv(glGetUniformLocation(shader.programId, "MVP"), 1, GL_FALSE, &MVP[0][0]);
	texture.Use();
	glBindVertexArray(this->VAO);
	glDrawElements(GL_TRIANGLES, vertexCount, GL_UNSIGNED_INT, nullptr);
	glBindVertexArray(0);
}

/*template<typename Func>
void ObjectXYZUV::RenderMVP(Rendering::Shader& shader, Rendering::Texture& texture, const glm::mat4& MVP, Func func) const{
	shader.Use();
	glUniformMatrix4fv(glGetUniformLocation(shader.programId, "MVP"), 1, GL_FALSE, &MVP[0][0]);

	func(shader);

	texture.Use();
	glBindVertexArray(this->VAO);
	glDrawElements(GL_TRIANGLES, vertexCount, GL_UNSIGNED_INT, nullptr);
	glBindVertexArray(0);
}*/

void ObjectXYZUV::Dispose(){
	glBindVertexArray(this->VAO);
	glDeleteVertexArrays(1, &this->VAO);
	glBindVertexArray(0);
	glDeleteBuffers(1, &this->VBO);
	glDeleteBuffers(1, &this->EBO);
}

//ObjectXYZUVS
ObjectXYZUVS::ObjectXYZUVS(){}
ObjectXYZUVS::ObjectXYZUVS(const std::vector<float>& mesh){
	glGenVertexArrays(1, &this->VAO);
	glGenBuffers(1, &this->VBO);
	glGenBuffers(1, &this->EBO);
	
	const int attributeCount = 6;
	std::vector<int> indexes;
	indexes.resize(mesh.size() / attributeCount / 4 * 6);
	for (int i = 0, k = 0; i < indexes.size(); i+=6, k+=4) {
		indexes[i + 0] = k + 0;
		indexes[i + 1] = k + 1;
		indexes[i + 2] = k + 2;
		indexes[i + 3] = k + 0;
		indexes[i + 4] = k + 2;
		indexes[i + 5] = k + 3;
	}

	this->vertexCount = indexes.size();

	glBindVertexArray(this->VAO);

	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mesh.size(), mesh.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * indexes.size(), indexes.data(), GL_STATIC_DRAW);

	// XYZ координаты
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, attributeCount * sizeof(float), (GLvoid*)nullptr);
	glEnableVertexAttribArray(0);
	
	// UV Текстурные координаты
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, attributeCount * sizeof(float), (GLvoid*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// S Тень
	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, attributeCount * sizeof(float), (GLvoid*)(5 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
}

void ObjectXYZUVS::ChangeMesh(const std::vector<float>& mesh){
	const int attributeCount = 6;
	std::vector<int> indexes;
	indexes.resize(mesh.size() / attributeCount / 4 * 6);
	for (int i = 0, k = 0; i < indexes.size(); i+=6, k+=4) {
		indexes[i + 0] = k + 0;
		indexes[i + 1] = k + 1;
		indexes[i + 2] = k + 2;
		indexes[i + 3] = k + 0;
		indexes[i + 4] = k + 2;
		indexes[i + 5] = k + 3;
	}

	this->vertexCount = indexes.size();

	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mesh.size(), mesh.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * indexes.size(), indexes.data(), GL_STATIC_DRAW);
}

ObjectXYZUVS::ObjectXYZUVS(const ObjectXYZUVS &another){
	this->VAO = another.VAO;
	this->VBO = another.VBO;
	this->EBO = another.EBO;
	this->vertexCount = another.vertexCount;
}

ObjectXYZUVS::ObjectXYZUVS(const tst::SimpleObj &another){
	this->VAO = another.VAO;
	this->VBO = another.VBO;
	this->EBO = another.EBO;
	this->vertexCount = another.vertexCount;
}

ObjectXYZUVS::ObjectXYZUVS(const Rendering::Batch& batch){
	this->VAO = batch.VAO;
	this->VBO = batch.VBO;
	this->EBO = batch.EBO;
	this->vertexCount = batch.vertexCount;
}

ObjectXYZUVS::ObjectXYZUVS(std::string modelFileName){
	std::vector<float> vertices;
	ImportObjN(modelFileName, vertices);

	glGenVertexArrays(1, &this->VAO);
	glGenBuffers(1, &this->VBO);
	glGenBuffers(1, &this->EBO);
	
	const int attributeCount = 6;

	std::vector<int> indexes;
	indexes.resize(vertices.size() / attributeCount);
	for (int i = 0; i < indexes.size(); i++) {
		indexes[i] = i;
	}

	this->vertexCount = indexes.size();

	glBindVertexArray(this->VAO);

	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * indexes.size(), indexes.data(), GL_STATIC_DRAW);

	// XYZ координаты
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, attributeCount * sizeof(float), (GLvoid*)nullptr);
	glEnableVertexAttribArray(0);
	
	// UV Текстурные координаты
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, attributeCount * sizeof(float), (GLvoid*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// S Тень
	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, attributeCount * sizeof(float), (GLvoid*)(5 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
}

void ObjectXYZUVS::RenderMVP(Rendering::Shader& shader, Rendering::Texture& texture, const glm::mat4& MVP) const{
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	shader.Use();
	glUniformMatrix4fv(glGetUniformLocation(shader.programId, "MVP"), 1, GL_FALSE, &MVP[0][0]);
	texture.Use();
	glBindVertexArray(this->VAO);
	glDrawElements(GL_TRIANGLES, vertexCount, GL_UNSIGNED_INT, nullptr);
	glBindVertexArray(0);

	//glDisable(GL_BLEND);
}

/*template<typename Func>
void ObjectXYZUVS::RenderMVP(Rendering::Shader& shader, Rendering::Texture& texture, const glm::mat4& MVP, Func func) const{
	shader.Use();
	glUniformMatrix4fv(glGetUniformLocation(shader.programId, "MVP"), 1, GL_FALSE, &MVP[0][0]);

	func(shader);

	texture.Use();
	glBindVertexArray(this->VAO);
	glDrawElements(GL_TRIANGLES, vertexCount, GL_UNSIGNED_INT, nullptr);
	glBindVertexArray(0);
}*/

void ObjectXYZUVS::Dispose(){
	glBindVertexArray(this->VAO);
	glDeleteVertexArrays(1, &this->VAO);
	glBindVertexArray(0);
	glDeleteBuffers(1, &this->VBO);
	glDeleteBuffers(1, &this->EBO);
}

//DynamicQuadXYZ
DynamicQuadXYZ::DynamicQuadXYZ(int quadCount)
{
	this->size = quadCount;

	glGenVertexArrays(1, &this->VAO);
	glGenBuffers(1, &this->VBO);
	glGenBuffers(1, &this->EBO);

	std::vector<int> indexes(6 * this->size);
	int k = 0;
	for (int i = 0; i < indexes.size(); i+=6) {
		indexes[i] = k;
		indexes[i + 1] = k + 2;
		indexes[i + 2] = k + 1;
		indexes[i + 3] = k + 2;
		indexes[i + 4] = k;
		indexes[i + 5] = k + 3;
		k += 4;
	}

	this->vertexCount = indexes.size();

	glBindVertexArray(this->VAO);

	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 4 * attributeCount * size, nullptr, GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * indexes.size(), indexes.data(), GL_STATIC_DRAW);

	// XYZ COORDS
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, attributeCount * sizeof(float), (GLvoid*)nullptr);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
}

void DynamicQuadXYZ::SetVertexData(const std::vector<float>& vertices)
{	
	if (vertices.size() != attributeCount * 4 * size)
		throw "Incorrect Size of a vertices array!";

	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * vertices.size(), vertices.data());

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void DynamicQuadXYZ::Render(Rendering::Shader& shader, const glm::mat4& MVP)
{
	shader.Use();
	glUniformMatrix4fv(glGetUniformLocation(shader.programId, "MVP"), 1, GL_FALSE, &MVP[0][0]);
	glBindVertexArray(this->VAO);
	glDrawElements(GL_TRIANGLES, this->vertexCount, GL_UNSIGNED_INT, nullptr);
	glBindVertexArray(0);
}

void DynamicQuadXYZ::RenderOnTop(Rendering::Shader& shader, const glm::mat4& MVP)
{
	glDepthFunc(GL_EQUAL);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	shader.Use();
	glUniformMatrix4fv(glGetUniformLocation(shader.programId, "MVP"), 1, GL_FALSE, &MVP[0][0]);
	glBindVertexArray(this->VAO);
	glDrawElements(GL_TRIANGLES, this->vertexCount, GL_UNSIGNED_INT, nullptr);
	glBindVertexArray(0);

	glDisable(GL_BLEND);
	glDepthFunc(GL_LESS);
}

void DynamicQuadXYZ::Dispose()
{
	glBindVertexArray(this->VAO);
	glDeleteVertexArrays(1, &this->VAO);
	glBindVertexArray(0);
	glDeleteBuffers(1, &this->VBO);
	glDeleteBuffers(1, &this->EBO);
}