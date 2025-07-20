#ifndef ENDLESS_CAVESS_CHUNK_H
#define ENDLESS_CAVESS_CHUNK_H

#include <vector>
#include <cmath>
#include <iostream>
#include <fstream>
#include <string>

#include "MyGraphicsExtensions.h"

static const int CHUNK_SIZE_X = 16;//32;
static const int CHUNK_SIZE_Y = 16;//48;
static const int CHUNK_SIZE_Z = 16;//32;

static const char FACE_X_F = 0b000001;
static const char FACE_X_B = 0b000010;
static const char FACE_Y_F = 0b000100;
static const char FACE_Y_B = 0b001000;
static const char FACE_Z_F = 0b010000;
static const char FACE_Z_B = 0b100000;

void AddFace(std::vector<float>& vertices, int block, int x, int y, int z, char face);
void AddFace(std::vector<float>& vertices, int block, int x, int y, int z, char face, float shadow[4]);
void AddCube(std::vector<float>& vertices, int block, int x, int y, int z, float size);

//void TryAddFace(std::vector<float>& vertices, int block, int x, int y, int z, 
//	bool uu, bool u_, bool ul, bool _u, bool __, bool _l, bool lu, bool l_, bool ll, char face);

#define TO_CHUNK_COORD(coord, coord_max) ( (coord) < 0 ? ((coord) + 1) / (coord_max) - 1 : (coord) / (coord_max) )
#define TO_CHUNK_COORD_RAW_FLOAT(raw_coord, coord_max) ( (raw_coord) < -0.5f ? (static_cast<int>(std::round( (raw_coord) )) + 1) / (coord_max) - 1 : static_cast<int>(std::round( (raw_coord) )) / (coord_max) )

#define TO_LOCAL_BLOCK_COORD(block_coord, chunk_coord, coord_max) ( (block_coord) - (coord_max) * (chunk_coord) )

class World;

class Chunk{
public:
	int x{0};
	int y{0};
	int z{0};
	int blocks[CHUNK_SIZE_X][CHUNK_SIZE_Y][CHUNK_SIZE_Z];

	bool isRendering = false;

	ObjectXYZUVS mesh;
	bool isDirty = true;

	int GetBlock(int x, int y, int z) const;

	void SetBlock(int block, int x, int y, int z);

	void InitializeMesh();

	void UpdateMesh();

	void InitializeMesh(World& world);

	void UpdateMesh(World& world);

	std::vector<float> CreateMesh() const;

	std::vector<float> CreateMesh2() const;

	std::vector<float> CreateMesh2(World& world) const;

	void Pair(Chunk& chunk);

	void Render(Rendering::Shader& shader, Rendering::Texture& texture, const glm::mat4& MVP) const;

	void SaveToFile(const std::string& worldDir) const;
    
    bool LoadFromFile(const std::string& worldDir);

private:
	std::string GetChunkFileName(const std::string& worldDir) const;
};

bool CheckCollision(const Chunk& chunk, float min_x, float min_y, float min_z, float max_x, float max_y, float max_z);

template <int chunk_x, int chunk_y, int chunk_z>
inline constexpr int GET_BLOCK(int local_x, int local_y, int local_z, Chunk* neighbors[3][3][3]) {
    if constexpr (chunk_x == 1 && chunk_y == 1 && chunk_z == 1) {
        return neighbors[chunk_x][chunk_y][chunk_z]->blocks[local_x][local_y][local_z];
    } else {
        return neighbors[chunk_x][chunk_y][chunk_z] != nullptr && 
               neighbors[chunk_x][chunk_y][chunk_z]->blocks[local_x][local_y][local_z];
    }
}

template <int chunk_x, int chunk_y, int chunk_z>
inline constexpr bool CHECK_SOLID(int local_x, int local_y, int local_z, Chunk* neighbors[3][3][3]) {
    if constexpr (chunk_x == 1 && chunk_y == 1 && chunk_z == 1) {
        return neighbors[chunk_x][chunk_y][chunk_z]->blocks[local_x][local_y][local_z] != 0;
    } else {
        return neighbors[chunk_x][chunk_y][chunk_z] != nullptr && 
               neighbors[chunk_x][chunk_y][chunk_z]->blocks[local_x][local_y][local_z] != 0;
    }
}

inline bool CHECK_SOLID(int chunk_x, int chunk_y, int chunk_z, int local_x, int local_y, int local_z, Chunk* neighbors[3][3][3]) {
    if (chunk_x == 1 && chunk_y == 1 && chunk_z == 1) {
        return neighbors[chunk_x][chunk_y][chunk_z]->blocks[local_x][local_y][local_z] != 0;
    } else {
        return neighbors[chunk_x][chunk_y][chunk_z] != nullptr && 
               neighbors[chunk_x][chunk_y][chunk_z]->blocks[local_x][local_y][local_z] != 0;
    }
}

template <int chunk_x, int chunk_y, int chunk_z>
inline constexpr bool CHECK_TRANSPARENT(int local_x, int local_y, int local_z, Chunk* neighbors[3][3][3]) {
    if constexpr (chunk_x == 1 && chunk_y == 1 && chunk_z == 1) {
        return neighbors[chunk_x][chunk_y][chunk_z]->blocks[local_x][local_y][local_z] == 0;
    } else {
        return neighbors[chunk_x][chunk_y][chunk_z] != nullptr && 
               neighbors[chunk_x][chunk_y][chunk_z]->blocks[local_x][local_y][local_z] == 0;
    }
}

inline bool CHECK_TRANSPARENT(int chunk_x, int chunk_y, int chunk_z, int local_x, int local_y, int local_z, Chunk* neighbors[3][3][3]) {
    if (chunk_x == 1 && chunk_y == 1 && chunk_z == 1) {
        return neighbors[chunk_x][chunk_y][chunk_z]->blocks[local_x][local_y][local_z] == 0;
    } else {
        return neighbors[chunk_x][chunk_y][chunk_z] != nullptr && 
               neighbors[chunk_x][chunk_y][chunk_z]->blocks[local_x][local_y][local_z] == 0;
    }
}

#endif //ENDLESS_CAVESS_CHUNK_H