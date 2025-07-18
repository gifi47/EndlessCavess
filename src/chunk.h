#ifndef ENDLESS_CAVESS_CHUNK_H
#define ENDLESS_CAVESS_CHUNK_H

#include <vector>
#include <cmath>
#include <iostream>
#include <fstream>
#include <string>

#include "MyGraphicsExtensions.h"

static const int CHUNK_SIZE_X = 32;
static const int CHUNK_SIZE_Y = 48;
static const int CHUNK_SIZE_Z = 32;

static const char FACE_X_F = 0b000001;
static const char FACE_X_B = 0b000010;
static const char FACE_Y_F = 0b000100;
static const char FACE_Y_B = 0b001000;
static const char FACE_Z_F = 0b010000;
static const char FACE_Z_B = 0b100000;

void AddFace(std::vector<float>& vertices, int block, int x, int y, int z, char face);
void AddFace(std::vector<float>& vertices, int block, int x, int y, int z, char face, float shadow[4]);
void AddCube(std::vector<float>& vertices, int block, int x, int y, int z, float size);

#define TO_CHUNK_COORD(coord, coord_max) ( (coord) < 0 ? ((coord) + 1) / (coord_max) - 1 : (coord) / (coord_max) )
#define TO_CHUNK_COORD_RAW_FLOAT(raw_coord, coord_max) ( (raw_coord) < -0.5f ? (static_cast<int>(std::round( (raw_coord) )) + 1) / (coord_max) - 1 : static_cast<int>(std::round( (raw_coord) )) / (coord_max) )

#define TO_LOCAL_BLOCK_COORD(block_coord, chunk_coord, coord_max) ( (block_coord) - (coord_max) * (chunk_coord) )

class ChunkStorage;

class Chunk{
public:
	int x{0};
	int y{0};
	int z{0};
	int blocks[CHUNK_SIZE_X][CHUNK_SIZE_Y][CHUNK_SIZE_Z];

	bool isRendering = false;

	ObjectXYZUVS mesh;
	bool isDirty = true;

	int GetBlock(int x, int y, int z) const{
		return blocks[TO_LOCAL_BLOCK_COORD(x, this->x, CHUNK_SIZE_X)][TO_LOCAL_BLOCK_COORD(y, this->y, CHUNK_SIZE_Y)][TO_LOCAL_BLOCK_COORD(z, this->z, CHUNK_SIZE_Z)];
	}

	void SetBlock(int block, int x, int y, int z){
		if (blocks[TO_LOCAL_BLOCK_COORD(x, this->x, CHUNK_SIZE_X)][TO_LOCAL_BLOCK_COORD(y, this->y, CHUNK_SIZE_Y)][TO_LOCAL_BLOCK_COORD(z, this->z, CHUNK_SIZE_Z)] == block) return;
		blocks[TO_LOCAL_BLOCK_COORD(x, this->x, CHUNK_SIZE_X)][TO_LOCAL_BLOCK_COORD(y, this->y, CHUNK_SIZE_Y)][TO_LOCAL_BLOCK_COORD(z, this->z, CHUNK_SIZE_Z)] = block;
		isDirty = true;
	}

	void InitializeMesh(){
		std::vector<float> mesh_verts = CreateMesh2();

		if (mesh_verts.size() == 0) isRendering = false;
		else isRendering = true;

		mesh = ObjectXYZUVS(mesh_verts);
		isDirty = false;
	}

	void UpdateMesh(){
		if (isDirty){
			std::vector<float> mesh_verts = CreateMesh2();

			if (mesh_verts.size() == 0) isRendering = false;
			else isRendering = true;

			mesh.ChangeMesh(mesh_verts);
			isDirty = false;
		}
	}

	std::vector<float> CreateMesh() const{
		std::vector<float> vertices(0);

		for (int _x = 0; _x < CHUNK_SIZE_X; _x++){
			for (int _y = 0; _y < CHUNK_SIZE_Y; _y++){
				for (int _z = 0; _z < CHUNK_SIZE_Z; _z++){
					int block = blocks[_x][_y][_z];
					if (block != 0) {
						if (_x == 0 || blocks[_x-1][_y][_z] == 0){
							AddFace(vertices, block, _x+x*CHUNK_SIZE_X, _y+y*CHUNK_SIZE_Y, _z+z*CHUNK_SIZE_Z, FACE_X_B);
						}
						if (_x == (CHUNK_SIZE_X - 1) || blocks[_x+1][_y][_z] == 0){
							AddFace(vertices, block, _x+x*CHUNK_SIZE_X, _y+y*CHUNK_SIZE_Y, _z+z*CHUNK_SIZE_Z, FACE_X_F);
						}
						if (_y == 0 || blocks[_x][_y-1][_z] == 0){
							AddFace(vertices, block, _x+x*CHUNK_SIZE_X, _y+y*CHUNK_SIZE_Y, _z+z*CHUNK_SIZE_Z, FACE_Y_B);
						}
						if (_y == (CHUNK_SIZE_Y - 1) || blocks[_x][_y+1][_z] == 0){
							AddFace(vertices, block, _x+x*CHUNK_SIZE_X, _y+y*CHUNK_SIZE_Y, _z+z*CHUNK_SIZE_Z, FACE_Y_F);
						}
						if (_z == 0 || blocks[_x][_y][_z-1] == 0){
							AddFace(vertices, block, _x+x*CHUNK_SIZE_X, _y+y*CHUNK_SIZE_Y, _z+z*CHUNK_SIZE_Z, FACE_Z_B);
						}
						if (_z == (CHUNK_SIZE_Z - 1) || blocks[_x][_y][_z+1] == 0){
							AddFace(vertices, block, _x+x*CHUNK_SIZE_X, _y+y*CHUNK_SIZE_Y, _z+z*CHUNK_SIZE_Z, FACE_Z_F);
						}
					}
				}
			}
		}
		return vertices;
	}

	std::vector<float> CreateMesh2() const{
		std::vector<float> vertices(0);
		float shadow[4] = {0.05f, 0.05f, 0.05f, 0.05f};
		bool isXl, isXu, isYl, isYu, isZl, isZu;

		for (int _x = 0; _x < CHUNK_SIZE_X; _x++){
			for (int _y = 0; _y < CHUNK_SIZE_Y; _y++){
				for (int _z = 0; _z < CHUNK_SIZE_Z; _z++){
					int block = blocks[_x][_y][_z];
					if (block != 0) {
						isXu = _x != (CHUNK_SIZE_X - 1);
						isXl = _x != 0;
						isYu = _y != (CHUNK_SIZE_Y - 1);
						isYl = _y != 0;
						isZu = _z != (CHUNK_SIZE_Z - 1);
						isZl = _z != 0;
						if (!isXl || blocks[_x-1][_y][_z] == 0){
							shadow[0] = 0.05f; shadow[1] = 0.05f; shadow[2] = 0.05f; shadow[3] = 0.05f;
							if (isXl){
								if (isYu && isZu && blocks[_x - 1][_y + 1][_z + 1] != 0) shadow[0] += 0.1f;
								if (isYu && isZl && blocks[_x - 1][_y + 1][_z - 1] != 0) shadow[1] += 0.1f;
								if (isYl && isZl && blocks[_x - 1][_y - 1][_z - 1] != 0) shadow[2] += 0.1f;
								if (isYl && isZu && blocks[_x - 1][_y - 1][_z + 1] != 0) shadow[3] += 0.1f;

								if (isYu && blocks[_x - 1][_y + 1][_z] != 0) {
									shadow[0] += 0.1f;
									shadow[1] += 0.1f;
								}
								if (isYl && blocks[_x - 1][_y - 1][_z] != 0) {
									shadow[2] += 0.1f;
									shadow[3] += 0.1f;
								}
								if (isZu && blocks[_x - 1][_y][_z + 1] != 0) {
									shadow[0] += 0.1f;
									shadow[3] += 0.1f;
								}
								if (isZl && blocks[_x - 1][_y][_z - 1] != 0) {
									shadow[1] += 0.1f;
									shadow[2] += 0.1f;
								}
							}
							AddFace(vertices, block, _x+x*CHUNK_SIZE_X, _y+y*CHUNK_SIZE_Y, _z+z*CHUNK_SIZE_Z, FACE_X_B, shadow);
						}
						if (!isXu || blocks[_x+1][_y][_z] == 0){
							shadow[0] = 0.05f; shadow[1] = 0.05f; shadow[2] = 0.05f; shadow[3] = 0.05f;
							if (isXu){
								if (isYu && isZl && blocks[_x + 1][_y + 1][_z - 1] != 0) shadow[0] += 0.1f;
								if (isYu && isZu && blocks[_x + 1][_y + 1][_z + 1] != 0) shadow[1] += 0.1f;
								if (isYl && isZu && blocks[_x + 1][_y - 1][_z + 1] != 0) shadow[2] += 0.1f;
								if (isYl && isZl && blocks[_x + 1][_y - 1][_z - 1] != 0) shadow[3] += 0.1f;

								if (isYu && blocks[_x + 1][_y + 1][_z] != 0) {
									shadow[0] += 0.1f;
									shadow[1] += 0.1f;
								}
								if (isYl && blocks[_x + 1][_y - 1][_z] != 0) {
									shadow[2] += 0.1f;
									shadow[3] += 0.1f;
								}
								if (isZu && blocks[_x + 1][_y][_z + 1] != 0) {
									shadow[1] += 0.1f;
									shadow[2] += 0.1f;
								}
								if (isZl && blocks[_x + 1][_y][_z - 1] != 0) {
									shadow[0] += 0.1f;
									shadow[3] += 0.1f;
								}
							}
							AddFace(vertices, block, _x+x*CHUNK_SIZE_X, _y+y*CHUNK_SIZE_Y, _z+z*CHUNK_SIZE_Z, FACE_X_F, shadow);
						}
						if (!isYl || blocks[_x][_y-1][_z] == 0){
							shadow[0] = 0.15f; shadow[1] = 0.15f; shadow[2] = 0.15f; shadow[3] = 0.15f;
							if (isYl){
								if (isXl && isZu && blocks[_x - 1][_y - 1][_z + 1] != 0) shadow[0] += 0.1f;
								if (isXl && isZl && blocks[_x - 1][_y - 1][_z - 1] != 0) shadow[1] += 0.1f;
								if (isXu && isZl && blocks[_x + 1][_y - 1][_z - 1] != 0) shadow[2] += 0.1f;
								if (isXu && isZu && blocks[_x + 1][_y - 1][_z + 1] != 0) shadow[3] += 0.1f;

								if (isXu && blocks[_x + 1][_y - 1][_z] != 0) {
									shadow[2] += 0.1f;
									shadow[3] += 0.1f;
								}
								if (isXl && blocks[_x - 1][_y - 1][_z] != 0) {
									shadow[0] += 0.1f;
									shadow[1] += 0.1f;
								}
								if (isZu && blocks[_x][_y - 1][_z + 1] != 0) {
									shadow[0] += 0.1f;
									shadow[3] += 0.1f;
								}
								if (isZl && blocks[_x][_y - 1][_z - 1] != 0) {
									shadow[1] += 0.1f;
									shadow[2] += 0.1f;
								}
							}
							AddFace(vertices, block, _x+x*CHUNK_SIZE_X, _y+y*CHUNK_SIZE_Y, _z+z*CHUNK_SIZE_Z, FACE_Y_B, shadow);
						}
						if (!isYu || blocks[_x][_y+1][_z] == 0){
							shadow[0] = 0.0f; shadow[1] = 0.0f; shadow[2] = 0.0f; shadow[3] = 0.0f;
							if (isYu){
								if (isXl && isZl && blocks[_x - 1][_y + 1][_z - 1] != 0) shadow[0] += 0.1f;
								if (isXl && isZu && blocks[_x - 1][_y + 1][_z + 1] != 0) shadow[1] += 0.1f;
								if (isXu && isZu && blocks[_x + 1][_y + 1][_z + 1] != 0) shadow[2] += 0.1f;
								if (isXu && isZl && blocks[_x + 1][_y + 1][_z - 1] != 0) shadow[3] += 0.1f;

								if (isXu && blocks[_x + 1][_y + 1][_z] != 0) {
									shadow[2] += 0.1f;
									shadow[3] += 0.1f;
								}
								if (isXl && blocks[_x - 1][_y + 1][_z] != 0) {
									shadow[0] += 0.1f;
									shadow[1] += 0.1f;
								}
								if (isZu && blocks[_x][_y + 1][_z + 1] != 0) {
									shadow[1] += 0.1f;
									shadow[2] += 0.1f;
								}
								if (isZl && blocks[_x][_y + 1][_z - 1] != 0) {
									shadow[0] += 0.1f;
									shadow[3] += 0.1f;
								}
							}
							AddFace(vertices, block, _x+x*CHUNK_SIZE_X, _y+y*CHUNK_SIZE_Y, _z+z*CHUNK_SIZE_Z, FACE_Y_F, shadow);
						}
						if (!isZl || blocks[_x][_y][_z-1] == 0){
							shadow[0] = 0.05f; shadow[1] = 0.05f; shadow[2] = 0.05f; shadow[3] = 0.05f;
							if (isZl){
								if (isYu && isXl && blocks[_x - 1][_y + 1][_z - 1] != 0) shadow[0] += 0.1f;
								if (isYu && isXu && blocks[_x + 1][_y + 1][_z - 1] != 0) shadow[1] += 0.1f;
								if (isYl && isXu && blocks[_x + 1][_y - 1][_z - 1] != 0) shadow[2] += 0.1f;
								if (isYl && isXl && blocks[_x - 1][_y - 1][_z - 1] != 0) shadow[3] += 0.1f;

								if (isYu && blocks[_x][_y + 1][_z - 1] != 0) {
									shadow[0] += 0.1f;
									shadow[1] += 0.1f;
								}
								if (isYl && blocks[_x][_y - 1][_z - 1] != 0) {
									shadow[2] += 0.1f;
									shadow[3] += 0.1f;
								}
								if (isXu && blocks[_x + 1][_y][_z - 1] != 0) {
									shadow[1] += 0.1f;
									shadow[2] += 0.1f;
								}
								if (isXl && blocks[_x - 1][_y][_z - 1] != 0) {
									shadow[0] += 0.1f;
									shadow[3] += 0.1f;
								}
							}
							AddFace(vertices, block, _x+x*CHUNK_SIZE_X, _y+y*CHUNK_SIZE_Y, _z+z*CHUNK_SIZE_Z, FACE_Z_B, shadow);
						}
						if (!isZu || blocks[_x][_y][_z+1] == 0){
							shadow[0] = 0.05f; shadow[1] = 0.05f; shadow[2] = 0.05f; shadow[3] = 0.05f;
							if (isZu){
								if (isYu && isXu && blocks[_x + 1][_y + 1][_z + 1] != 0) shadow[0] += 0.1f;
								if (isYu && isXl && blocks[_x - 1][_y + 1][_z + 1] != 0) shadow[1] += 0.1f;
								if (isYl && isXl && blocks[_x - 1][_y - 1][_z + 1] != 0) shadow[2] += 0.1f;
								if (isYl && isXu && blocks[_x + 1][_y - 1][_z + 1] != 0) shadow[3] += 0.1f;

								if (isYu && blocks[_x][_y + 1][_z + 1] != 0) {
									shadow[0] += 0.1f;
									shadow[1] += 0.1f;
								}
								if (isYl && blocks[_x][_y - 1][_z + 1] != 0) {
									shadow[2] += 0.1f;
									shadow[3] += 0.1f;
								}
								if (isXu && blocks[_x + 1][_y][_z + 1] != 0) {
									shadow[0] += 0.1f;
									shadow[3] += 0.1f;
								}
								if (isXl && blocks[_x - 1][_y][_z + 1] != 0) {
									shadow[1] += 0.1f;
									shadow[2] += 0.1f;
								}
							}
							AddFace(vertices, block, _x+x*CHUNK_SIZE_X, _y+y*CHUNK_SIZE_Y, _z+z*CHUNK_SIZE_Z, FACE_Z_F, shadow);
						}
					}
				}
			}
		}
		return vertices;
	}

	// TODO: add support for checking adjacent blocks from nearby loaded chunks! 
	/*std::vector<float> CreateMesh3(ChunkStorage& chunks) const{
		std::vector<float> vertices(0);
		float shadow[4] = {0.05f, 0.05f, 0.05f, 0.05f};

		for (int _x = 1; _x < (CHUNK_SIZE_X - 1); _x++){
			for (int _y = 1; _y < (CHUNK_SIZE_Y - 1); _y++){
				for (int _z = 1; _z < (CHUNK_SIZE_Z - 1); _z++){
					int block = blocks[_x][_y][_z];
					if (block != 0) {
						if (blocks[_x-1][_y][_z] == 0){
							shadow[0] = 0.05f; shadow[1] = 0.05f; shadow[2] = 0.05f; shadow[3] = 0.05f;
							if (blocks[_x - 1][_y + 1][_z + 1] != 0) shadow[0] += 0.1f;
							if (blocks[_x - 1][_y + 1][_z - 1] != 0) shadow[1] += 0.1f;
							if (blocks[_x - 1][_y - 1][_z - 1] != 0) shadow[2] += 0.1f;
							if (blocks[_x - 1][_y - 1][_z + 1] != 0) shadow[3] += 0.1f;

							if (blocks[_x - 1][_y + 1][_z] != 0) {
								shadow[0] += 0.1f;
								shadow[1] += 0.1f;
							}
							if (blocks[_x - 1][_y - 1][_z] != 0) {
								shadow[2] += 0.1f;
								shadow[3] += 0.1f;
							}
							if (blocks[_x - 1][_y][_z + 1] != 0) {
								shadow[0] += 0.1f;
								shadow[3] += 0.1f;
							}
							if (blocks[_x - 1][_y][_z - 1] != 0) {
								shadow[1] += 0.1f;
								shadow[2] += 0.1f;
							}
							AddFace(vertices, block, _x+x*CHUNK_SIZE_X, _y+y*CHUNK_SIZE_Y, _z+z*CHUNK_SIZE_Z, FACE_X_B, shadow);
						}
						if (blocks[_x+1][_y][_z] == 0){
							shadow[0] = 0.05f; shadow[1] = 0.05f; shadow[2] = 0.05f; shadow[3] = 0.05f;
							if (blocks[_x + 1][_y + 1][_z - 1] != 0) shadow[0] += 0.1f;
							if (blocks[_x + 1][_y + 1][_z + 1] != 0) shadow[1] += 0.1f;
							if (blocks[_x + 1][_y - 1][_z + 1] != 0) shadow[2] += 0.1f;
							if (blocks[_x + 1][_y - 1][_z - 1] != 0) shadow[3] += 0.1f;

							if (blocks[_x + 1][_y + 1][_z] != 0) {
								shadow[0] += 0.1f;
								shadow[1] += 0.1f;
							}
							if (blocks[_x + 1][_y - 1][_z] != 0) {
								shadow[2] += 0.1f;
								shadow[3] += 0.1f;
							}
							if (blocks[_x + 1][_y][_z + 1] != 0) {
								shadow[1] += 0.1f;
								shadow[2] += 0.1f;
							}
							if (blocks[_x + 1][_y][_z - 1] != 0) {
								shadow[0] += 0.1f;
								shadow[3] += 0.1f;
							}
							AddFace(vertices, block, _x+x*CHUNK_SIZE_X, _y+y*CHUNK_SIZE_Y, _z+z*CHUNK_SIZE_Z, FACE_X_F, shadow);
						}
						if (blocks[_x][_y-1][_z] == 0){
							shadow[0] = 0.15f; shadow[1] = 0.15f; shadow[2] = 0.15f; shadow[3] = 0.15f;
							if (blocks[_x - 1][_y - 1][_z + 1] != 0) shadow[0] += 0.1f;
							if (blocks[_x - 1][_y - 1][_z - 1] != 0) shadow[1] += 0.1f;
							if (blocks[_x + 1][_y - 1][_z - 1] != 0) shadow[2] += 0.1f;
							if (blocks[_x + 1][_y - 1][_z + 1] != 0) shadow[3] += 0.1f;

							if (blocks[_x + 1][_y - 1][_z] != 0) {
								shadow[2] += 0.1f;
								shadow[3] += 0.1f;
							}
							if (blocks[_x - 1][_y - 1][_z] != 0) {
								shadow[0] += 0.1f;
								shadow[1] += 0.1f;
							}
							if (blocks[_x][_y - 1][_z + 1] != 0) {
								shadow[0] += 0.1f;
								shadow[3] += 0.1f;
							}
							if (blocks[_x][_y - 1][_z - 1] != 0) {
								shadow[1] += 0.1f;
								shadow[2] += 0.1f;
							}
							AddFace(vertices, block, _x+x*CHUNK_SIZE_X, _y+y*CHUNK_SIZE_Y, _z+z*CHUNK_SIZE_Z, FACE_Y_B, shadow);
						}
						if (blocks[_x][_y+1][_z] == 0){
							shadow[0] = 0.0f; shadow[1] = 0.0f; shadow[2] = 0.0f; shadow[3] = 0.0f;
							if (blocks[_x - 1][_y + 1][_z - 1] != 0) shadow[0] += 0.1f;
							if (blocks[_x - 1][_y + 1][_z + 1] != 0) shadow[1] += 0.1f;
							if (blocks[_x + 1][_y + 1][_z + 1] != 0) shadow[2] += 0.1f;
							if (blocks[_x + 1][_y + 1][_z - 1] != 0) shadow[3] += 0.1f;

							if (blocks[_x + 1][_y + 1][_z] != 0) {
								shadow[2] += 0.1f;
								shadow[3] += 0.1f;
							}
							if (blocks[_x - 1][_y + 1][_z] != 0) {
								shadow[0] += 0.1f;
								shadow[1] += 0.1f;
							}
							if (blocks[_x][_y + 1][_z + 1] != 0) {
								shadow[1] += 0.1f;
								shadow[2] += 0.1f;
							}
							if (blocks[_x][_y + 1][_z - 1] != 0) {
								shadow[0] += 0.1f;
								shadow[3] += 0.1f;
							}
							AddFace(vertices, block, _x+x*CHUNK_SIZE_X, _y+y*CHUNK_SIZE_Y, _z+z*CHUNK_SIZE_Z, FACE_Y_F, shadow);
						}
						if (blocks[_x][_y][_z-1] == 0){
							shadow[0] = 0.05f; shadow[1] = 0.05f; shadow[2] = 0.05f; shadow[3] = 0.05f;

							if (blocks[_x - 1][_y + 1][_z - 1] != 0) shadow[0] += 0.1f;
							if (blocks[_x + 1][_y + 1][_z - 1] != 0) shadow[1] += 0.1f;
							if (blocks[_x + 1][_y - 1][_z - 1] != 0) shadow[2] += 0.1f;
							if (blocks[_x - 1][_y - 1][_z - 1] != 0) shadow[3] += 0.1f;

							if (blocks[_x][_y + 1][_z - 1] != 0) {
								shadow[0] += 0.1f;
								shadow[1] += 0.1f;
							}
							if (blocks[_x][_y - 1][_z - 1] != 0) {
								shadow[2] += 0.1f;
								shadow[3] += 0.1f;
							}
							if (blocks[_x + 1][_y][_z - 1] != 0) {
								shadow[1] += 0.1f;
								shadow[2] += 0.1f;
							}
							if (blocks[_x - 1][_y][_z - 1] != 0) {
								shadow[0] += 0.1f;
								shadow[3] += 0.1f;
							}
							
							AddFace(vertices, block, _x+x*CHUNK_SIZE_X, _y+y*CHUNK_SIZE_Y, _z+z*CHUNK_SIZE_Z, FACE_Z_B, shadow);
						}
						if (blocks[_x][_y][_z+1] == 0){
							shadow[0] = 0.05f; shadow[1] = 0.05f; shadow[2] = 0.05f; shadow[3] = 0.05f;

							if (blocks[_x + 1][_y + 1][_z + 1] != 0) shadow[0] += 0.1f;
							if (blocks[_x - 1][_y + 1][_z + 1] != 0) shadow[1] += 0.1f;
							if (blocks[_x - 1][_y - 1][_z + 1] != 0) shadow[2] += 0.1f;
							if (blocks[_x + 1][_y - 1][_z + 1] != 0) shadow[3] += 0.1f;

							if (blocks[_x][_y + 1][_z + 1] != 0) {
								shadow[0] += 0.1f;
								shadow[1] += 0.1f;
							}
							if (blocks[_x][_y - 1][_z + 1] != 0) {
								shadow[2] += 0.1f;
								shadow[3] += 0.1f;
							}
							if (blocks[_x + 1][_y][_z + 1] != 0) {
								shadow[0] += 0.1f;
								shadow[3] += 0.1f;
							}
							if (blocks[_x - 1][_y][_z + 1] != 0) {
								shadow[1] += 0.1f;
								shadow[2] += 0.1f;
							}
							
							AddFace(vertices, block, _x+x*CHUNK_SIZE_X, _y+y*CHUNK_SIZE_Y, _z+z*CHUNK_SIZE_Z, FACE_Z_F, shadow);
						}
					}
				}
			}
		}

		Chunk* chunk_ptr = chunks.GetChunk(x, y, z - 1);
		for (int _x = 1; _x < CHUNK_SIZE_X - 1; _x++){

			if (chunk_ptr != nullptr){
				if (blocks[])
			}
		}

		return vertices;
	}*/

	void Pair(Chunk& chunk){
		isRendering = false;
	}

	void Render(Rendering::Shader& shader, Rendering::Texture& texture, const glm::mat4& MVP) const{
		if (isRendering)
			mesh.RenderMVP(shader, texture, MVP);
	}

	void SaveToFile(const std::string& worldDir) const {
        std::string filename = GetChunkFileName(worldDir);
        std::ofstream file(filename, std::ios::binary);
        if (!file) return;
        
        file.write(reinterpret_cast<const char*>(blocks), sizeof(blocks));
    }
    
    bool LoadFromFile(const std::string& worldDir) {
        std::string filename = GetChunkFileName(worldDir);
        std::ifstream file(filename, std::ios::binary);
        if (!file) return false;

        file.read(reinterpret_cast<char*>(blocks), sizeof(blocks));
        //std::cout << "Loaded chunk: " << x << " " << y << " " << z << "\n";
        return true;
    }

private:
	std::string GetChunkFileName(const std::string& worldDir) const {
        return worldDir + "/chunk_" + std::to_string(x) + "_" + std::to_string(y) + "_" + std::to_string(z) + ".bin";
    }
};

bool CheckCollision(const Chunk& chunk, float min_x, float min_y, float min_z, float max_x, float max_y, float max_z);

#endif //ENDLESS_CAVESS_CHUNK_H