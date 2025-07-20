#ifndef WORLD_H
#define WORLD_H

#ifdef __cpp_lib_filesystem

#include <filesystem>

#define CREATE_DIRECTORY(dir_name) std::filesystem::create_directories( (dir_name) )

#else

#ifdef _WIN32

#define ERROR_ALREADY_EXISTS 183
//#include <windows.h>
extern "C" __declspec(dllimport) int __stdcall CreateDirectoryA(const char* lpPathName, void* lpSecurityAttributes);
extern "C" __declspec(dllimport) unsigned long __stdcall GetLastError();
#define CREATE_DIRECTORY(dir_name) ( CreateDirectoryA( (dir_name).c_str(), NULL) != 0 || GetLastError() == ERROR_ALREADY_EXISTS )

#else

#include <sys/stat.h>
#include <sys/types.h>
#define CREATE_DIRECTORY(dir_name) ( mkdir( (dir_name).c_str(), 0755) == 0 || errno == EEXIST )

#endif //_WIN32

#endif //__cpp_lib_filesystem

#include <string>
#include <vector>
#include <cmath>

#include <GL/glew.h>

#include "Rendering/Shader.h"
#include "Rendering/Texture.h"
#include <glm/glm.hpp>

#include "chunk.h"
#include "blocks.h"
#include "noise.h"

#define CHUNK_STORAGE_MAX_CHUNKS 1024 * 32

/*
	class ChunkStorage

	TODO: 	Replace this class with std::map 
			or something similar for better 
			access to chunks by their coords.
*/
class ChunkStorage{
public:
	std::string worldName;
	ChunkStorage(const std::string& name) : worldName(name) {
		chunks = new Chunk[CHUNK_STORAGE_MAX_CHUNKS];
	}
	~ChunkStorage(){
		delete[] chunks;
	}
	Chunk* chunks;//[CHUNK_STORAGE_MAX_CHUNKS];

	int loadedChunks = 0;

	using iterator = Chunk*;
    using const_iterator = const Chunk*;

	iterator begin() { return chunks; }
    iterator end() { return chunks + loadedChunks; }

    const_iterator begin() const { return chunks; }
    const_iterator end() const { return chunks + loadedChunks; }
    const_iterator cbegin() const { return chunks; }
    const_iterator cend() const { return chunks + loadedChunks; }

	Chunk* GetChunk(int x, int y, int z){
		for (int i = 0; i < loadedChunks; i++){
			if (chunks[i].x == x && chunks[i].y == y && chunks[i].z == z) return &chunks[i];
		}
		return nullptr;
	}

	Chunk* AddChunk(const Chunk& chunk){
		if (loadedChunks < CHUNK_STORAGE_MAX_CHUNKS){
			loadedChunks++;
			chunks[loadedChunks - 1] = chunk;
			//std::cout << "loadedChunks: " << loadedChunks << "\n";
			return &chunks[loadedChunks - 1];
		}
		constexpr int half = CHUNK_STORAGE_MAX_CHUNKS / 2;
		for (int i = 0; i < half; i++){
			chunks[i].mesh.Dispose();
			chunks[i].SaveToFile(worldName); // TEMPORARY SOLUTION
			chunks[i] = chunks[i + half];
		}
		chunks[half] = chunk;
		loadedChunks = half + 1;
		//std::cout << "loadedChunks: " << loadedChunks << "\n";
		return &chunks[half];
	}

	void RemoveChunk(int x, int y, int z){
		for (int i = 0; i < loadedChunks; i++){
			if (chunks[i].x == x && chunks[i].y == y && chunks[i].z == z) {

				chunks[i].mesh.Dispose();
				chunks[i].SaveToFile(worldName);

				for (int j = i + 1; j < loadedChunks; j++){
					chunks[j - 1] = chunks[j];
				}

				loadedChunks -= 1;
				break;
			}
		}
	}

	void RemoveChunks(const std::vector<int>& sorted_inds){
		if (sorted_inds.size() < 1) return;
		int k = 0;
		int e = 0;
		for (int i = 0; i < loadedChunks; i++){
			if (k < sorted_inds.size() && i == (sorted_inds[k])){
				k++;
				chunks[i].mesh.Dispose();
				chunks[i].SaveToFile(worldName);
			} else {
				chunks[e++] = chunks[i];
			}
		}
		loadedChunks -= k;
	}
};

class World
{
public:

	std::string name;
	long long seed;
	ChunkStorage chunks;
	noise::SimpleNoise simpleNoise;
	noise::SimpleNoise simpleNoise2;
	noise::SimpleNoise3D simpleNoise3d;

	World(const std::string& name): 
		name(name), 
		simpleNoise(30, 30, 2341414), 
		simpleNoise2(54, 54, 31), 
		simpleNoise3d(121, 134, 121, 14),
		chunks(name)
	{
        CREATE_DIRECTORY(name);
    }

	int GetBlock(int x, int y, int z){
		int chunk_x = TO_CHUNK_COORD(x, CHUNK_SIZE_X);
		int chunk_y = TO_CHUNK_COORD(y, CHUNK_SIZE_Y);
		int chunk_z = TO_CHUNK_COORD(z, CHUNK_SIZE_Z);
		Chunk* chunk_ptr = chunks.GetChunk(chunk_x, chunk_y, chunk_z);
		if (chunk_ptr == nullptr){
			chunk_ptr = GetChunk(chunk_x, chunk_y, chunk_z);
		}
		return chunk_ptr->GetBlock(x, y, z);
	}

	void SetBlock(int block, int x, int y, int z){
		int chunk_x = TO_CHUNK_COORD(x, CHUNK_SIZE_X);
		int chunk_y = TO_CHUNK_COORD(y, CHUNK_SIZE_Y);
		int chunk_z = TO_CHUNK_COORD(z, CHUNK_SIZE_Z);
		Chunk* chunk_ptr = chunks.GetChunk(chunk_x, chunk_y, chunk_z);
		if (chunk_ptr == nullptr){
			chunk_ptr = GetChunk(chunk_x, chunk_y, chunk_z);
		}
		chunk_ptr->SetBlock(block, x, y, z);
	}

	Chunk* GetChunk(int x, int y, int z){
		Chunk* chunk_ptr = chunks.GetChunk(x, y, z);
		if (chunk_ptr == nullptr){
			chunk_ptr = LoadChunk(x, y, z);
		}
		return chunk_ptr;
	}

	Chunk* GetChunkNoLoad(int x, int y, int z){
		Chunk* chunk_ptr = chunks.GetChunk(x, y, z);
		return chunk_ptr;
	}

	Chunk* LoadChunk(int x, int y, int z){
		Chunk chunk;
		chunk.x = x;
		chunk.y = y;
		chunk.z = z;
		if (!chunk.LoadFromFile(name)) {
            chunk = CreateChunk(x, y, z);
        } else {
        	chunk.InitializeMesh();
        }
		return chunks.AddChunk(chunk);
	}

	/*Chunk CreateChunk(int x, int y, int z){
		Chunk chunk;
		chunk.x = x;
		chunk.y = y;
		chunk.z = z;
		for (int _x = 0; _x < CHUNK_SIZE_X; _x++){
			for (int _z = 0; _z < CHUNK_SIZE_Z; _z++){
				int half = CHUNK_SIZE_Y / 2;
				for (int _y = half; _y < CHUNK_SIZE_Y; _y++){
					chunk.blocks[_x][_y][_z] = 0;
				}
				chunk.blocks[_x][half - 1][_z] = static_cast<int>(BlockType::Grass);
				for (int _y = 2; _y < half - 1; _y++){
					chunk.blocks[_x][_y][_z] = static_cast<int>(BlockType::Dirt);
				}
				for (int _y = 0; _y < 2; _y++){
					chunk.blocks[_x][_y][_z] = static_cast<int>(BlockType::Stone);
				}
			}
		}
		chunk.InitializeMesh();
		return chunk;
	}*/

	Chunk CreateChunk(int x, int y, int z){
		Chunk chunk;
		chunk.x = x;
		chunk.y = y;
		chunk.z = z;

		for (int _x = 0; _x < CHUNK_SIZE_X; _x++){
			for (int _z = 0; _z < CHUNK_SIZE_Z; _z++){
				float y_level_f = simpleNoise.GetValue((_x + x * CHUNK_SIZE_X + 512) / 1024.0f, (_z + z * CHUNK_SIZE_Z + 512) / 1024.0f);

				float y_level_f2 = simpleNoise2.GetValue((_x + x * CHUNK_SIZE_X + 512) / 1024.0f, (_z + z * CHUNK_SIZE_Z + 512) / 1024.0f);

				y_level_f = noise::smooth(0.2f, y_level_f, y_level_f2);

				int y_level = static_cast<int>(y_level_f * 64) - 32;
				bool water_level = (y * CHUNK_SIZE_Y < 0);
				if (water_level){
					for (int _y = 0; _y < CHUNK_SIZE_Y; _y++){
						chunk.blocks[_x][_y][_z] = static_cast<int>(BlockType::Water);
					}
				} else {
					for (int _y = 0; _y < CHUNK_SIZE_Y; _y++){
						chunk.blocks[_x][_y][_z] = 0;
					}
				}

				int local_height = y_level - y * CHUNK_SIZE_Y + 1;

				if (local_height > 0){
					if (local_height <= CHUNK_SIZE_Y){
						if (water_level)
							chunk.blocks[_x][local_height - 1][_z] = static_cast<int>(BlockType::Sand);
						else
							chunk.blocks[_x][local_height - 1][_z] = static_cast<int>(BlockType::Grass);
					}
				}
				if (local_height > 1){
					if ((local_height - 1) < CHUNK_SIZE_Y){
						if (water_level)
							chunk.blocks[_x][local_height - 2][_z] = static_cast<int>(BlockType::Sand);
						else
							chunk.blocks[_x][local_height - 2][_z] = static_cast<int>(BlockType::Dirt);
					}
				}
				if (local_height > 2){
					if ((local_height - 2) < CHUNK_SIZE_Y){
						if (water_level)
							chunk.blocks[_x][local_height - 3][_z] = static_cast<int>(BlockType::Sand);
						else
							chunk.blocks[_x][local_height - 3][_z] = static_cast<int>(BlockType::Dirt);
					}
				}
				if (local_height > 3){
					for (int _y = 0; _y < std::min(local_height - 3, CHUNK_SIZE_Y); _y++){
						chunk.blocks[_x][_y][_z] = static_cast<int>(BlockType::Stone);
					}
				}

				for (int _y = 0; _y < std::min(local_height, CHUNK_SIZE_Y); _y++){
					if (chunk.blocks[_x][_y][_z] != static_cast<int>(BlockType::Water) && chunk.blocks[_x][_y][_z] != 0){
						float n3d = simpleNoise3d.GetValue((_x + x * CHUNK_SIZE_X + 512) / 1024.0f, 
							(_z + z * CHUNK_SIZE_Z + 512) / 1024.0f, (_y + y * CHUNK_SIZE_Y + 512) / 1024.0f);
						if (n3d < 0.2f){
							chunk.blocks[_x][_y][_z] = 0;
						}
						else if (n3d < 0.21f){
							chunk.blocks[_x][_y][_z] = static_cast<int>(BlockType::Dirt);
						}
					}
				}
			}
		}

		chunk.InitializeMesh(*this);
		return chunk;
	}

	void RenderChunks(Rendering::Shader& shader, Rendering::Texture& texture, const glm::mat4& MVP) const{
		for (Chunk chunk : chunks) {
			chunk.Render(shader, texture, MVP);
		}
	}

	void UpdateChunks(){
		for (Chunk& chunk : chunks) {
			chunk.UpdateMesh(*this);
		}
	}

	void SaveAllChunks() {
        for (Chunk& chunk : chunks) {
            chunk.SaveToFile(name);
        }
    }

    void LoadNearbyChunks(float x, float y, float z, int radius=10){
    	int chunk_x = TO_CHUNK_COORD(x, CHUNK_SIZE_X);
		int chunk_y = TO_CHUNK_COORD(y, CHUNK_SIZE_Y);
		int chunk_z = TO_CHUNK_COORD(z, CHUNK_SIZE_Z);
    	for (int _x = chunk_x - radius; _x <= chunk_x+radius; _x++){
    		for (int _y = chunk_y - radius; _y <= chunk_y+radius; _y++){
    			for (int _z = chunk_z - radius; _z <= chunk_z+radius; _z++){
    				GetChunk(_x, _y, _z);
    			}
    		}
    	}
    }
    void LoadNearbyChunks(float x, float y, float z, int radius_x, int radius_y, int radius_z){
    	int chunk_x = TO_CHUNK_COORD(x, CHUNK_SIZE_X);
		int chunk_y = TO_CHUNK_COORD(y, CHUNK_SIZE_Y);
		int chunk_z = TO_CHUNK_COORD(z, CHUNK_SIZE_Z);
    	for (int _x = chunk_x - radius_x; _x <= chunk_x+ radius_x; _x++){
    		for (int _y = chunk_y - radius_y; _y <= chunk_y+radius_y; _y++){
    			for (int _z = chunk_z - radius_z; _z <= chunk_z+radius_z; _z++){
    				GetChunk(_x, _y, _z);
    			}
    		}
    	}
    }

    void UnloadEmptyChunks(){
    	std::vector<int> toUnload(0);
    	int k = 0;
    	for (Chunk& chunk : chunks) {
    		if (!chunk.isRendering){
    			toUnload.push_back(k);
    		}
    		k++;
    	}
    	chunks.RemoveChunks(toUnload);
    }

    void UnloadHiddenChunks(float x, float y, float z){
    	int chunk_x = TO_CHUNK_COORD(x, CHUNK_SIZE_X);
		int chunk_y = TO_CHUNK_COORD(y, CHUNK_SIZE_Y);
		int chunk_z = TO_CHUNK_COORD(z, CHUNK_SIZE_Z);
		
		std::vector<int> toUnload(0);
    	int k = 0;
    	for (Chunk& chunk : chunks) {
    		int mod_x = chunk_x - chunk.x, mod_y, mod_z;
    		if (mod_x != 0) {
    			mod_x = glm::sign(mod_x);

    			Chunk* chunk_ptr = chunks.GetChunk(chunk.x + mod_x, chunk.y, chunk.z);
	            if (chunk_ptr != nullptr){
	            	int check_pos = (mod_x == -1 ? CHUNK_SIZE_X - 1 : 0);
	            	for (int _y = 0; _y < CHUNK_SIZE_Y; _y++){
	            		for (int _z = 0; _z < CHUNK_SIZE_Z; _z++){
	            			if (chunk_ptr->blocks[check_pos][_y][_z] == 0)
	            			goto __ExitUnloadHiddenChunks__; // CHUNK IS VISIBLE
	            		}
	            	}
	            }
    		}
    		
    		mod_y = chunk_y - chunk.y;
    		if (mod_y != 0) {
    			mod_y = glm::sign(mod_y);

    			Chunk* chunk_ptr = chunks.GetChunk(chunk.x, chunk.y + mod_y, chunk.z);
	            if (chunk_ptr != nullptr){
	            	int check_pos = (mod_y == -1 ? CHUNK_SIZE_Y - 1 : 0);
	            	for (int _x = 0; _x < CHUNK_SIZE_X; _x++){
	            		for (int _z = 0; _z < CHUNK_SIZE_Z; _z++){
	            			if (chunk_ptr->blocks[_x][check_pos][_z] == 0)
	            			goto __ExitUnloadHiddenChunks__; // CHUNK IS VISIBLE
	            		}
	            	}
	            }
    		}

    		mod_z = chunk_z - chunk.z;
    		if (mod_z != 0) {

    			mod_z = glm::sign(mod_z);

	            Chunk* chunk_ptr = chunks.GetChunk(chunk.x, chunk.y, chunk.z + mod_z);
	            if (chunk_ptr != nullptr){
	            	int check_pos = (mod_z == -1 ? CHUNK_SIZE_Z - 1 : 0);
	            	for (int _x = 0; _x < CHUNK_SIZE_X; _x++){
	            		for (int _y = 0; _y < CHUNK_SIZE_Y; _y++){
	            			if (chunk_ptr->blocks[_x][_y][check_pos] == 0)
	            			goto __ExitUnloadHiddenChunks__; // CHUNK IS VISIBLE
	            		}
	            	}
	            } 
        	}
        	if (!(mod_x == 0 && mod_y == 0 && mod_z == 0))
        		toUnload.push_back(k);
        	__ExitUnloadHiddenChunks__:
        	k++;
        }
        chunks.RemoveChunks(toUnload);
    }

	bool CheckCollision(float min_x, float min_y, float min_z, float max_x, float max_y, float max_z){
		int chunk_min_x = TO_CHUNK_COORD_RAW_FLOAT(min_x, CHUNK_SIZE_X);
		int chunk_min_y = TO_CHUNK_COORD_RAW_FLOAT(min_y, CHUNK_SIZE_Y);
		int chunk_min_z = TO_CHUNK_COORD_RAW_FLOAT(min_z, CHUNK_SIZE_Z);
		int chunk_max_x = TO_CHUNK_COORD_RAW_FLOAT(max_x, CHUNK_SIZE_X);
		int chunk_max_y = TO_CHUNK_COORD_RAW_FLOAT(max_y, CHUNK_SIZE_Y);
		int chunk_max_z = TO_CHUNK_COORD_RAW_FLOAT(max_z, CHUNK_SIZE_Z);
		for (Chunk& chunk : chunks) {
			for (int _x = chunk_min_x; _x <= chunk_max_x; _x++){
				for (int _y = chunk_min_y; _y <= chunk_max_y; _y++){
					for (int _z = chunk_min_z; _z <= chunk_max_z; _z++){
						if (::CheckCollision(chunk, min_x, min_y, min_z, max_x, max_y, max_z)) return true;
					}
				}
			}
		}
		return false;
	}
};

#endif //WORLD_H