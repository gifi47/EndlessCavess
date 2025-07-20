#include "chunk.h"
#include "world.h"

#define add_coords(x_p, y_p, z_p) {vertices.push_back(x_p);vertices.push_back(y_p);vertices.push_back(z_p);}
#define add_uv(u_p, v_p) {vertices.push_back(u_p);vertices.push_back(v_p);}
#define add_shadow(s_v) {vertices.push_back( (s_v) );}

void AddFace(std::vector<float>& vertices, int block, int x, int y, int z, char face){
	int u = (block & 0b00001111);
	int v = ((block & 0b11110000) >> 4);

	int w = 16; // texture size
    int h = 16;
    float minu = u * 1.0f/w,        minv = v * 1.0f/h;
    float maxu = minu + 1.0f/w,     maxv = minv + 1.0f/h;

    float bs = 0.5f;

	switch (face){
	case FACE_X_B:
		add_coords(x - bs, y + bs, z + bs); add_uv(minu, maxv);
		add_coords(x - bs, y + bs, z - bs); add_uv(maxu, maxv);
		add_coords(x - bs, y - bs, z - bs); add_uv(maxu, minv);
		add_coords(x - bs, y - bs, z + bs); add_uv(minu, minv);
		break;
	case FACE_X_F:
		add_coords(x + bs, y + bs, z - bs); add_uv(minu, maxv);
		add_coords(x + bs, y + bs, z + bs); add_uv(maxu, maxv);
		add_coords(x + bs, y - bs, z + bs); add_uv(maxu, minv);
		add_coords(x + bs, y - bs, z - bs); add_uv(minu, minv);
		break;
	case FACE_Y_B:
		add_coords(x - bs, y - bs, z + bs); add_uv(minu, maxv);
		add_coords(x - bs, y - bs, z - bs); add_uv(maxu, maxv);
		add_coords(x + bs, y - bs, z - bs); add_uv(maxu, minv);
		add_coords(x + bs, y - bs, z + bs); add_uv(minu, minv);
		break;
	case FACE_Y_F:
		add_coords(x - bs, y + bs, z - bs); add_uv(minu, maxv);
		add_coords(x - bs, y + bs, z + bs); add_uv(maxu, maxv);
		add_coords(x + bs, y + bs, z + bs); add_uv(maxu, minv);
		add_coords(x + bs, y + bs, z - bs); add_uv(minu, minv);
		break;
	case FACE_Z_B:
		add_coords(x - bs, y + bs, z - bs); add_uv(minu, maxv);
		add_coords(x + bs, y + bs, z - bs); add_uv(maxu, maxv);
		add_coords(x + bs, y - bs, z - bs); add_uv(maxu, minv);
		add_coords(x - bs, y - bs, z - bs); add_uv(minu, minv);
		break;
	case FACE_Z_F:
		add_coords(x + bs, y + bs, z + bs); add_uv(minu, maxv);
		add_coords(x - bs, y + bs, z + bs); add_uv(maxu, maxv);
		add_coords(x - bs, y - bs, z + bs); add_uv(maxu, minv);
		add_coords(x + bs, y - bs, z + bs); add_uv(minu, minv);
		break;
	}
}

void AddFace(std::vector<float>& vertices, int block, int x, int y, int z, char face, float shadow[4]){
	int u = (block & 0b00001111);
	int v = ((block & 0b11110000) >> 4);

	int w = 16; // texture size
    int h = 16;
    float minu = u * 1.0f/w,        minv = v * 1.0f/h;
    float maxu = minu + 1.0f/w,     maxv = minv + 1.0f/h;

    float bs = 0.5f;
    //std::cout << shadow[0] << " @ " << shadow[1] << " @ " << shadow[2] << " @ " << shadow[3] << "\n";
	switch (face){
	case FACE_X_B:
		add_coords(x - bs, y + bs, z + bs); add_uv(minu, maxv); add_shadow(shadow[0]);
		add_coords(x - bs, y + bs, z - bs); add_uv(maxu, maxv); add_shadow(shadow[1]);
		add_coords(x - bs, y - bs, z - bs); add_uv(maxu, minv); add_shadow(shadow[2]);
		add_coords(x - bs, y - bs, z + bs); add_uv(minu, minv); add_shadow(shadow[3]);
		break;
	case FACE_X_F:
		add_coords(x + bs, y + bs, z - bs); add_uv(minu, maxv); add_shadow(shadow[0]);
		add_coords(x + bs, y + bs, z + bs); add_uv(maxu, maxv); add_shadow(shadow[1]);
		add_coords(x + bs, y - bs, z + bs); add_uv(maxu, minv); add_shadow(shadow[2]);
		add_coords(x + bs, y - bs, z - bs); add_uv(minu, minv); add_shadow(shadow[3]);
		break;
	case FACE_Y_B:
		add_coords(x - bs, y - bs, z + bs); add_uv(minu, maxv); add_shadow(shadow[0]);
		add_coords(x - bs, y - bs, z - bs); add_uv(maxu, maxv); add_shadow(shadow[1]);
		add_coords(x + bs, y - bs, z - bs); add_uv(maxu, minv); add_shadow(shadow[2]);
		add_coords(x + bs, y - bs, z + bs); add_uv(minu, minv); add_shadow(shadow[3]);
		break;
	case FACE_Y_F:
		add_coords(x - bs, y + bs, z - bs); add_uv(minu, maxv); add_shadow(shadow[0]);
		add_coords(x - bs, y + bs, z + bs); add_uv(maxu, maxv); add_shadow(shadow[1]);
		add_coords(x + bs, y + bs, z + bs); add_uv(maxu, minv); add_shadow(shadow[2]);
		add_coords(x + bs, y + bs, z - bs); add_uv(minu, minv); add_shadow(shadow[3]);
		break;
	case FACE_Z_B:
		add_coords(x - bs, y + bs, z - bs); add_uv(minu, maxv); add_shadow(shadow[0]);
		add_coords(x + bs, y + bs, z - bs); add_uv(maxu, maxv); add_shadow(shadow[1]);
		add_coords(x + bs, y - bs, z - bs); add_uv(maxu, minv); add_shadow(shadow[2]);
		add_coords(x - bs, y - bs, z - bs); add_uv(minu, minv); add_shadow(shadow[3]);
		break;
	case FACE_Z_F:
		add_coords(x + bs, y + bs, z + bs); add_uv(minu, maxv); add_shadow(shadow[0]);
		add_coords(x - bs, y + bs, z + bs); add_uv(maxu, maxv); add_shadow(shadow[1]);
		add_coords(x - bs, y - bs, z + bs); add_uv(maxu, minv); add_shadow(shadow[2]);
		add_coords(x + bs, y - bs, z + bs); add_uv(minu, minv); add_shadow(shadow[3]);
		break;
	}
}

void AddCube(std::vector<float>& vertices, int block, int x, int y, int z, float size=0.5f){
	int u = (block & 0b00001111);
	int v = ((block & 0b11110000) >> 4);

	int w = 16; // texture size
    int h = 16;
    float minu = u * 1.0f/w,        minv = v * 1.0f/h;
    float maxu = minu + 1.0f/w,     maxv = minv + 1.0f/h;

    float bs = size;

	add_coords(x - bs, y + bs, z + bs); add_uv(minu, maxv);
	add_coords(x - bs, y + bs, z - bs); add_uv(maxu, maxv);
	add_coords(x - bs, y - bs, z - bs); add_uv(maxu, minv);
	add_coords(x - bs, y - bs, z + bs); add_uv(minu, minv);

	add_coords(x + bs, y + bs, z - bs); add_uv(minu, maxv);
	add_coords(x + bs, y + bs, z + bs); add_uv(maxu, maxv);
	add_coords(x + bs, y - bs, z + bs); add_uv(maxu, minv);
	add_coords(x + bs, y - bs, z - bs); add_uv(minu, minv);

	add_coords(x - bs, y - bs, z + bs); add_uv(minu, maxv);
	add_coords(x - bs, y - bs, z - bs); add_uv(maxu, maxv);
	add_coords(x + bs, y - bs, z - bs); add_uv(maxu, minv);
	add_coords(x + bs, y - bs, z + bs); add_uv(minu, minv);

	add_coords(x - bs, y + bs, z - bs); add_uv(minu, maxv);
	add_coords(x - bs, y + bs, z + bs); add_uv(maxu, maxv);
	add_coords(x + bs, y + bs, z + bs); add_uv(maxu, minv);
	add_coords(x + bs, y + bs, z - bs); add_uv(minu, minv);

	add_coords(x - bs, y + bs, z - bs); add_uv(minu, maxv);
	add_coords(x + bs, y + bs, z - bs); add_uv(maxu, maxv);
	add_coords(x + bs, y - bs, z - bs); add_uv(maxu, minv);
	add_coords(x - bs, y - bs, z - bs); add_uv(minu, minv);

	add_coords(x + bs, y + bs, z + bs); add_uv(minu, maxv);
	add_coords(x - bs, y + bs, z + bs); add_uv(maxu, maxv);
	add_coords(x - bs, y - bs, z + bs); add_uv(maxu, minv);
	add_coords(x + bs, y - bs, z + bs); add_uv(minu, minv);
}


int Chunk::GetBlock(int x, int y, int z) const{
	return blocks[TO_LOCAL_BLOCK_COORD(x, this->x, CHUNK_SIZE_X)][TO_LOCAL_BLOCK_COORD(y, this->y, CHUNK_SIZE_Y)][TO_LOCAL_BLOCK_COORD(z, this->z, CHUNK_SIZE_Z)];
}

void Chunk::SetBlock(int block, int x, int y, int z){
	if (blocks[TO_LOCAL_BLOCK_COORD(x, this->x, CHUNK_SIZE_X)][TO_LOCAL_BLOCK_COORD(y, this->y, CHUNK_SIZE_Y)][TO_LOCAL_BLOCK_COORD(z, this->z, CHUNK_SIZE_Z)] == block) return;
	blocks[TO_LOCAL_BLOCK_COORD(x, this->x, CHUNK_SIZE_X)][TO_LOCAL_BLOCK_COORD(y, this->y, CHUNK_SIZE_Y)][TO_LOCAL_BLOCK_COORD(z, this->z, CHUNK_SIZE_Z)] = block;
	isDirty = true;
}

void Chunk::InitializeMesh(){
	std::vector<float> mesh_verts = CreateMesh2();

	if (mesh_verts.size() == 0) isRendering = false;
	else isRendering = true;

	mesh = ObjectXYZUVS(mesh_verts);
	isDirty = false;
}

void Chunk::UpdateMesh(){
	if (isDirty){
		std::vector<float> mesh_verts = CreateMesh2();

		if (mesh_verts.size() == 0) isRendering = false;
		else isRendering = true;

		mesh.ChangeMesh(mesh_verts);
		isDirty = false;
	}
}

void Chunk::InitializeMesh(World& world){
	std::vector<float> mesh_verts = CreateMesh2(world);

	if (mesh_verts.size() == 0) isRendering = false;
	else isRendering = true;

	mesh = ObjectXYZUVS(mesh_verts);
	isDirty = false;
}

void Chunk::UpdateMesh(World& world){
	if (isDirty){
		std::vector<float> mesh_verts = CreateMesh2(world);

		if (mesh_verts.size() == 0) isRendering = false;
		else isRendering = true;

		mesh.ChangeMesh(mesh_verts);
		isDirty = false;
	}
}

std::vector<float> Chunk::CreateMesh() const{
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

std::vector<float> Chunk::CreateMesh2() const{
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

std::vector<float> Chunk::CreateMesh2(World& world) const{
	std::vector<float> vertices(0);
	int blocks[CHUNK_SIZE_X + 2][CHUNK_SIZE_Y + 2][CHUNK_SIZE_Z + 2];

	Chunk* neighbors[3][3][3];
	for (int _x = 0; _x < 3; _x++){
		for (int _y = 0; _y < 3; _y++){
			for (int _z = 0; _z < 3; _z++){
				neighbors[_x][_y][_z] = world.GetChunkNoLoad(x + (_x - 1), y + (_y - 1), z + (_z - 1));
			}
		}
	}
	for (int _x = 1; _x <= CHUNK_SIZE_X; _x++){
		blocks[_x][0][0] = (neighbors[1][0][0] == nullptr ? 0 : neighbors[1][0][0]->blocks[_x - 1][CHUNK_SIZE_Y - 1][CHUNK_SIZE_Z - 1]);
		blocks[_x][CHUNK_SIZE_Y + 1][0] = (neighbors[1][2][0] == nullptr ? 0 : neighbors[1][2][0]->blocks[_x - 1][0][CHUNK_SIZE_Z - 1]);
		blocks[_x][CHUNK_SIZE_Y + 1][CHUNK_SIZE_Z + 1] = (neighbors[1][2][2] == nullptr ? 0 : neighbors[1][2][2]->blocks[_x - 1][0][0]);
		blocks[_x][0][CHUNK_SIZE_Z + 1] = (neighbors[1][0][2] == nullptr ? 0 : neighbors[1][0][2]->blocks[_x - 1][CHUNK_SIZE_Y - 1][0]);
		for (int _y = 1; _y <= CHUNK_SIZE_Y; _y++){
			blocks[_x][_y][0] = (neighbors[1][1][0] == nullptr ? 0 : neighbors[1][1][0]->blocks[_x - 1][_y - 1][CHUNK_SIZE_Z - 1]);
			blocks[_x][_y][CHUNK_SIZE_Z + 1] = (neighbors[1][1][2] == nullptr ? 0 : neighbors[1][1][2]->blocks[_x - 1][_y - 1][0]);
		}
	}
	for (int _y = 1; _y <= CHUNK_SIZE_Y; _y++){
		blocks[0][_y][0] = (neighbors[0][1][0] == nullptr ? 0 : neighbors[0][1][0]->blocks[CHUNK_SIZE_X - 1][_y - 1][CHUNK_SIZE_Z - 1]);
		blocks[CHUNK_SIZE_X + 1][_y][0] = (neighbors[2][1][0] == nullptr ? 0 : neighbors[2][1][0]->blocks[0][_y - 1][CHUNK_SIZE_Z - 1]);
		blocks[CHUNK_SIZE_X + 1][_y][CHUNK_SIZE_Z + 1] = (neighbors[2][1][2] == nullptr ? 0 : neighbors[2][1][2]->blocks[0][_y - 1][0]);
		blocks[0][_y][CHUNK_SIZE_Z + 1] = (neighbors[0][1][2] == nullptr ? 0 : neighbors[0][1][2]->blocks[CHUNK_SIZE_X - 1][_y - 1][0]);
		for (int _z = 1; _z <= CHUNK_SIZE_Z; _z++){
			blocks[0][_y][_z] = (neighbors[0][1][1] == nullptr ? 0 : neighbors[0][1][1]->blocks[CHUNK_SIZE_X - 1][_y - 1][_z - 1]);
			blocks[CHUNK_SIZE_X + 1][_y][_z] = (neighbors[2][1][1] == nullptr ? 0 : neighbors[2][1][1]->blocks[0][_y - 1][_z - 1]);
		}
	}
	for (int _z = 1; _z <= CHUNK_SIZE_Z; _z++){
		blocks[0][0][_z] = (neighbors[1][0][0] == nullptr ? 0 : neighbors[0][0][1]->blocks[CHUNK_SIZE_X - 1][CHUNK_SIZE_Y - 1][_z - 1]);
		blocks[CHUNK_SIZE_X + 1][0][_z] = (neighbors[1][0][0] == nullptr ? 0 : neighbors[2][0][1]->blocks[0][CHUNK_SIZE_Y - 1][_z - 1]);
		blocks[CHUNK_SIZE_X + 1][CHUNK_SIZE_Y + 1][_z] = (neighbors[1][0][0] == nullptr ? 0 : neighbors[2][2][1]->blocks[0][0][_z - 1]);
		blocks[0][CHUNK_SIZE_Y + 1][_z] = (neighbors[1][0][0] == nullptr ? 0 : neighbors[0][2][1]->blocks[CHUNK_SIZE_X - 1][0][_z - 1]);
		for (int _x = 1; _x <= CHUNK_SIZE_X; _x++){
			blocks[_x][0][_z] = (neighbors[1][0][1] == nullptr ? 0 : neighbors[1][0][1]->blocks[_x - 1][CHUNK_SIZE_Y - 1][_z - 1]);
			blocks[_x][CHUNK_SIZE_Y + 1][_z] = (neighbors[1][2][1] == nullptr ? 0 : neighbors[1][2][1]->blocks[_x - 1][0][_z - 1]);
		}
	}
	blocks[0][0][0] = (neighbors[0][0][0] == nullptr ? 0 : neighbors[0][0][0]->blocks[CHUNK_SIZE_X - 1][CHUNK_SIZE_Y - 1][CHUNK_SIZE_Z - 1]);
	blocks[0][0][CHUNK_SIZE_Z + 1] = (neighbors[0][0][2] == nullptr ? 0 : neighbors[0][0][2]->blocks[CHUNK_SIZE_X - 1][CHUNK_SIZE_Y - 1][0]);
	blocks[0][CHUNK_SIZE_Y + 1][0] = (neighbors[0][2][0] == nullptr ? 0 : neighbors[0][2][0]->blocks[CHUNK_SIZE_X - 1][0][CHUNK_SIZE_Z - 1]);
	blocks[0][CHUNK_SIZE_Y + 1][CHUNK_SIZE_Z + 1] = (neighbors[0][2][2] == nullptr ? 0 : neighbors[0][2][2]->blocks[CHUNK_SIZE_X - 1][0][0]);
	blocks[CHUNK_SIZE_X + 1][0][0] = (neighbors[2][0][0] == nullptr ? 0 : neighbors[2][0][0]->blocks[0][CHUNK_SIZE_Y - 1][CHUNK_SIZE_Z - 1]);
	blocks[CHUNK_SIZE_X + 1][0][CHUNK_SIZE_Z + 1] = (neighbors[2][0][2] == nullptr ? 0 : neighbors[2][0][2]->blocks[0][CHUNK_SIZE_Y - 1][0]);
	blocks[CHUNK_SIZE_X + 1][CHUNK_SIZE_Y + 1][0] = (neighbors[2][2][0] == nullptr ? 0 : neighbors[2][2][0]->blocks[0][0][CHUNK_SIZE_Z - 1]);
	blocks[CHUNK_SIZE_X + 1][CHUNK_SIZE_Y + 1][CHUNK_SIZE_Z + 1] = (neighbors[2][2][2] == nullptr ? 0 : neighbors[2][2][2]->blocks[0][0][0]);

	for (int _x = 0; _x < CHUNK_SIZE_X; _x++){
		for (int _y = 0; _y < CHUNK_SIZE_Y; _y++){
			for (int _z = 0; _z < CHUNK_SIZE_Z; _z++){
				blocks[_x + 1][_y + 1][_z + 1] = this->blocks[_x][_y][_z];
			}
		}
	}
	float shadow[4] = {0.05f, 0.05f, 0.05f, 0.05f};

	for (int _x = 1; _x <= CHUNK_SIZE_X; _x++){
		for (int _y = 1; _y <= CHUNK_SIZE_Y; _y++){
			for (int _z = 1; _z <= CHUNK_SIZE_Z; _z++){
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
						
						AddFace(vertices, block, (_x-1)+x*CHUNK_SIZE_X, (_y-1)+y*CHUNK_SIZE_Y, (_z-1)+z*CHUNK_SIZE_Z, FACE_X_B, shadow);
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
						
						AddFace(vertices, block, (_x-1)+x*CHUNK_SIZE_X, (_y-1)+y*CHUNK_SIZE_Y, (_z-1)+z*CHUNK_SIZE_Z, FACE_X_F, shadow);
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
						
						AddFace(vertices, block, (_x-1)+x*CHUNK_SIZE_X, (_y-1)+y*CHUNK_SIZE_Y, (_z-1)+z*CHUNK_SIZE_Z, FACE_Y_B, shadow);
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
						AddFace(vertices, block, (_x-1)+x*CHUNK_SIZE_X, (_y-1)+y*CHUNK_SIZE_Y, (_z-1)+z*CHUNK_SIZE_Z, FACE_Y_F, shadow);
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
						AddFace(vertices, block, (_x-1)+x*CHUNK_SIZE_X, (_y-1)+y*CHUNK_SIZE_Y, (_z-1)+z*CHUNK_SIZE_Z, FACE_Z_B, shadow);
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

						AddFace(vertices, block, (_x-1)+x*CHUNK_SIZE_X, (_y-1)+y*CHUNK_SIZE_Y, (_z-1)+z*CHUNK_SIZE_Z, FACE_Z_F, shadow);
					}
				}
			}
		}
	}
	return vertices;
}

	/*void s(){
		int nearbyBlocks[3][3][3];

		for (int _x = 1; _x < CHUNK_SIZE_X - 1; _x++){
			for (int _y = 1; _y < CHUNK_SIZE_Y - 1; _y++){
				TryAddFace(vertices, block[_x][_y][CHUNK_SIZE_Z - 1], 
					_x +x*CHUNK_SIZE_X, _y +y*CHUNK_SIZE_Y, (CHUNK_SIZE_Z - 1) +z*CHUNK_SIZE_Z, 
					CHECK_SOLID<1, 1, 2>(_x + 1, _y + 1, 0),
					CHECK_SOLID<1, 1, 2>(_x + 1, _y, 0),
					CHECK_SOLID<1, 1, 2>(_x + 1, _y - 1, 0),
					CHECK_SOLID<1, 1, 2>(_x, _y + 1, 0),
					CHECK_SOLID<1, 1, 2>(_x, _y, 0),
					CHECK_SOLID<1, 1, 2>(_x, _y - 1, 0),
					CHECK_SOLID<1, 1, 2>(_x - 1, _y + 1, 0),
					CHECK_SOLID<1, 1, 2>(_x - 1, _y, 0),
					CHECK_SOLID<1, 1, 2>(_x - 1, _y - 1, 0),
				FACE_Z_F);
				TryAddFace(vertices, block[_x][_y][0], 
					_x +x*CHUNK_SIZE_X, _y +y*CHUNK_SIZE_Y, 0 +z*CHUNK_SIZE_Z, 
					CHECK_SOLID<1, 1, 0>(_x + 1, _y + 1, 0),
					CHECK_SOLID<1, 1, 0>(_x + 1, _y, 0),
					CHECK_SOLID<1, 1, 0>(_x + 1, _y - 1, 0),
					CHECK_SOLID<1, 1, 0>(_x, _y + 1, 0),
					CHECK_SOLID<1, 1, 0>(_x, _y, 0),
					CHECK_SOLID<1, 1, 0>(_x, _y - 1, 0),
					CHECK_SOLID<1, 1, 0>(_x - 1, _y + 1, 0),
					CHECK_SOLID<1, 1, 0>(_x - 1, _y, 0),
					CHECK_SOLID<1, 1, 0>(_x - 1, _y - 1, 0),
				FACE_Z_B);
			}
		}

		for (int _x = 0; _x < CHUNK_SIZE_X; _x++){
			for (int _y = 0; _y < CHUNK_SIZE_Y; _y++){
				for (int _z = 0; _z < CHUNK_SIZE_Z; _z++){
					
					bool nearbyBlocks[3][3][3];
					for (int _xi = 0; _xi < 3; _xi++){
						for (int _yi = 0; _yi < 3; _yi++){
							for (int _zi = 0; _zi < 3; _zi++){
								 if ()
							}
						}						
					}


					for (int side = 0; side < 6; side++){
						
						for (int k = 0; k < 4; k++){

						}

					}

				}
			}
		}
	}
/*
#define XU 2
#define XL 0
#define YU 2
#define YL 0
#define ZU 2
#define ZL 0
#define __ 1

	std::vector<float> CreateMesh2(const World& world) const{
		std::vector<float> vertices(0);
		float shadow[4] = {0.05f, 0.05f, 0.05f, 0.05f};
		bool isXl, isXu, isYl, isYu, isZl, isZu;

		Chunk* neighbors[3][3][3];
		for (int _x = 0; _x < 3; _x++){
			for (int _y = 0; _y < 3; _y++){
				for (int _z = 0; _z < 3; _z++){
					neighbors[_x][_y][_z] = world.GetChunkNoLoad(x + (_x - 1), y + (_y - 1), z + (_z - 1));
				}
			}
		}

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
						if (isXl && blocks[_x-1][_y][_z] == 0 || !CHECK_SOLID<XL, __, __>(CHUNK_SIZE_X - 1, _y, _z, neighbors)){
							shadow[0] = 0.05f; shadow[1] = 0.05f; shadow[2] = 0.05f; shadow[3] = 0.05f;
							if (isXl){
								if (isYu) {
									if (blocks[_x - 1][_y + 1][_z] != 0) {
										shadow[0] += 0.1f;
										shadow[1] += 0.1f;
									}
									if (isZu && blocks[_x - 1][_y + 1][_z + 1] != 0) shadow[0] += 0.1f;
									if (isZl && blocks[_x - 1][_y + 1][_z - 1] != 0) shadow[1] += 0.1f;
								} else {
									if (CHECK_SOLID(__, YU, __, _x - 1, 0, _z)) {
										shadow[0] += 0.1f;
										shadow[1] += 0.1f;
									}
									if (!isZu && CHECK_SOLID(__, YU, ZU, _x - 1, 0, 0)) shadow[0] += 0.1f;
									if (!isZl && CHECK_SOLID(__, YU, ZL, _x - 1, 0, CHUNK_SIZE_Z - 1)) shadow[1] += 0.1f;
								}
								if (isYl) {
									if (blocks[_x - 1][_y - 1][_z] != 0) {
										shadow[2] += 0.1f;
										shadow[3] += 0.1f;
									}
									if (isZl && blocks[_x - 1][_y - 1][_z - 1] != 0) shadow[2] += 0.1f;
									if (isZu && blocks[_x - 1][_y - 1][_z + 1] != 0) shadow[3] += 0.1f;
								} else {
									if (CHECK_SOLID(__, YL, __, _x - 1, CHUNK_SIZE_Y - 1, _z)) {
										shadow[2] += 0.1f;
										shadow[3] += 0.1f;
									}
									if (!isZl && CHECK_SOLID(__, YL, ZL, _x - 1, CHUNK_SIZE_Y - 1, CHUNK_SIZE_Z - 1)) shadow[2] += 0.1f;
									if (!isZu && CHECK_SOLID(__, YL, ZU, _x - 1, CHUNK_SIZE_Y - 1, 0)) shadow[3] += 0.1f;
								}
								if (isZu) {
									if (blocks[_x - 1][_y][_z + 1] != 0) {
										shadow[0] += 0.1f;
										shadow[3] += 0.1f;
									}
								} else {
									if (CHECK_SOLID(__, __, ZU, _x - 1, _y, 0)) {
										shadow[0] += 0.1f;
										shadow[1] += 0.1f;
									}
								}
								if (isZl) 
									if (blocks[_x - 1][_y][_z - 1] != 0) {
										shadow[1] += 0.1f;
										shadow[2] += 0.1f;
									}
								} else {
									if (CHECK_SOLID(__, __, ZL, _x - 1, _y, CHUNK_SIZE_Z - 1)) {
										shadow[0] += 0.1f;
										shadow[1] += 0.1f;
									}
								}
							} else {
								if (isYu) {
									if (CHECK_SOLID(XL, __, __, CHUNK_SIZE_X - 1, _y + 1, _z)){
										shadow[0] += 0.1f;
										shadow[1] += 0.1f;
									}
									if (isZu && CHECK_SOLID(XL, __, __, CHUNK_SIZE_X - 1, _y + 1, _z + 1)) shadow[0] += 0.1f;
									if (isZl && CHECK_SOLID(XL, __, __, CHUNK_SIZE_X - 1, _y + 1, _z - 1)) shadow[1] += 0.1f;
								} else {
									if (CHECK_SOLID(XL, YU, __, CHUNK_SIZE_X - 1, 0, _z)) {
										shadow[0] += 0.1f;
										shadow[1] += 0.1f;
									}
									if (!isZu && CHECK_SOLID(XL, YU, ZU, CHUNK_SIZE_X - 1, 0, 0)) shadow[0] += 0.1f;
									if (!isZl && CHECK_SOLID(XL, YU, ZL, CHUNK_SIZE_X - 1, 0, CHUNK_SIZE_Z - 1)) shadow[1] += 0.1f;
								}
								if (isYl) {
									if (CHECK_SOLID(XL, __, __, CHUNK_SIZE_X - 1, _y - 1, _z)) {
										shadow[2] += 0.1f;
										shadow[3] += 0.1f;
									}
									if (isZl && CHECK_SOLID(XL, __, __, CHUNK_SIZE_X - 1, _y - 1, _z - 1)) shadow[2] += 0.1f;
									if (isZu && CHECK_SOLID(XL, __, __, CHUNK_SIZE_X - 1, _y - 1, _z + 1)) shadow[3] += 0.1f;
								} else {
									if (CHECK_SOLID(XL, YL, __, CHUNK_SIZE_X - 1, CHUNK_SIZE_Y - 1, _z)) {
										shadow[2] += 0.1f;
										shadow[3] += 0.1f;
									}
									if (!isZl && CHECK_SOLID(XL, YL, ZL, CHUNK_SIZE_X - 1, CHUNK_SIZE_Y - 1, CHUNK_SIZE_Z - 1)) shadow[2] += 0.1f;
									if (!isZu && CHECK_SOLID(XL, YL, ZU, CHUNK_SIZE_X - 1, CHUNK_SIZE_Y - 1, 0)) shadow[3] += 0.1f;
								}
								if (isZu) {
									if (CHECK_SOLID(XL, __, __, CHUNK_SIZE_X - 1, _y, _z + 1)) {
										shadow[0] += 0.1f;
										shadow[3] += 0.1f;
									}
								} else {
									if (CHECK_SOLID(XL, __, ZU, CHUNK_SIZE_X - 1, _y, 0)) {
										shadow[0] += 0.1f;
										shadow[1] += 0.1f;
									}
								}
								if (isZl) 
									if (CHECK_SOLID(XL, __, __, CHUNK_SIZE_X - 1, _y, _z - 1)) {
										shadow[1] += 0.1f;
										shadow[2] += 0.1f;
									}
								} else {
									if (CHECK_SOLID(XL, __, ZL, CHUNK_SIZE_X - 1, _y, CHUNK_SIZE_Z - 1)) {
										shadow[0] += 0.1f;
										shadow[1] += 0.1f;
									}
								}
							}
							AddFace(vertices, block, _x+x*CHUNK_SIZE_X, _y+y*CHUNK_SIZE_Y, _z+z*CHUNK_SIZE_Z, FACE_X_B, shadow);
						}
						if (isXu && blocks[_x+1][_y][_z] == 0 || CHECK_TRANSPARENT(XU, __, __, 0, _y, _z)){
							shadow[0] = 0.05f; shadow[1] = 0.05f; shadow[2] = 0.05f; shadow[3] = 0.05f;
							if (isXu){
								if (isYu) {
									if (blocks[_x + 1][_y + 1][_z] != 0) {
										shadow[0] += 0.1f;
										shadow[1] += 0.1f;
									}
									if (isZl && blocks[_x + 1][_y + 1][_z - 1] != 0) shadow[0] += 0.1f;
									if (isZu && blocks[_x + 1][_y + 1][_z + 1] != 0) shadow[1] += 0.1f;
								} else {
									if (CHECK_SOLID(__, YU, __, _x + 1, 0, _z)) {
										shadow[0] += 0.1f;
										shadow[1] += 0.1f;
									}
									if (!isZl && CHECK_SOLID(__, YU, ZL, _x + 1, 0, CHUNK_SIZE_Z - 1)) shadow[0] += 0.1f;
									if (!isZu && CHECK_SOLID(__, YU, ZU, _x + 1, 0, 0)) shadow[1] += 0.1f;
								}
								if (isYl) {
									if (blocks[_x + 1][_y - 1][_z] != 0) {
										shadow[2] += 0.1f;
										shadow[3] += 0.1f;
									}
									if (isZl && blocks[_x + 1][_y - 1][_z - 1] != 0) shadow[2] += 0.1f;
									if (isZu && blocks[_x + 1][_y - 1][_z + 1] != 0) shadow[3] += 0.1f;
								} else {
									if (CHECK_SOLID(__, YL, __, _x + 1, CHUNK_SIZE_Y - 1, _z)) {
										shadow[2] += 0.1f;
										shadow[3] += 0.1f;
									}
									if (!isZl && CHECK_SOLID(__, YL, ZL, _x + 1, CHUNK_SIZE_Y - 1, CHUNK_SIZE_Z - 1)) shadow[2] += 0.1f;
									if (!isZu && CHECK_SOLID(__, YL, ZU, _x + 1, CHUNK_SIZE_Y - 1, 0)) shadow[3] += 0.1f;
								}
								if (isZl) {
									if (blocks[_x + 1][_y][_z - 1] != 0) {
										shadow[0] += 0.1f;
										shadow[3] += 0.1f;
									}
								} else {
									if (CHECK_SOLID(__, __, ZL, _x + 1, _y, CHUNK_SIZE_Z - 1)) {
										shadow[0] += 0.1f;
										shadow[1] += 0.1f;
									}
								}
								if (isZu) {
									if (blocks[_x + 1][_y][_z + 1] != 0) {
										shadow[1] += 0.1f;
										shadow[2] += 0.1f;
									}
								} else {
									if (CHECK_SOLID(__, __, ZU, _x + 1, _y, 0)) {
										shadow[0] += 0.1f;
										shadow[1] += 0.1f;
									}
								}
							} else {
								if (isYu) {
									if (CHECK_SOLID(XU, __, __, 0, _y + 1, _z)) {
										shadow[0] += 0.1f;
										shadow[1] += 0.1f;
									}
									if (isZl && CHECK_SOLID(XU, __, ZL, 0, _y + 1, _z - 1)) shadow[0] += 0.1f;
									if (isZu && CHECK_SOLID(XU, __, ZU, 0, _y + 1, _z + 1)) shadow[1] += 0.1f;
								} else {
									if (CHECK_SOLID(XU, YU, __, 0, 0, _z)) {
										shadow[0] += 0.1f;
										shadow[1] += 0.1f;
									}
									if (!isZl && CHECK_SOLID(XU, YU, ZL, 0, 0, CHUNK_SIZE_Z - 1)) shadow[0] += 0.1f;
									if (!isZu && CHECK_SOLID(XU, YU, ZU, 0, 0, 0)) shadow[1] += 0.1f;
								}
								if (isYl) {
									if (CHECK_SOLID(XU, __, __, 0, _y - 1, _z)) {
										shadow[2] += 0.1f;
										shadow[3] += 0.1f;
									}
									if (isZl && CHECK_SOLID(XU, __, ZL, 0, _y - 1, _z - 1)) shadow[2] += 0.1f;
									if (isZu && CHECK_SOLID(XU, __, ZU, 0, _y - 1, _z + 1)) shadow[3] += 0.1f;
								} else {
									if (CHECK_SOLID(XU, YL, __, 0, CHUNK_SIZE_Y - 1, _z)) {
										shadow[2] += 0.1f;
										shadow[3] += 0.1f;
									}
									if (!isZl && CHECK_SOLID(XU, YL, ZL, 0, CHUNK_SIZE_Y - 1, _z - 1)) shadow[2] += 0.1f;
									if (!isZu && CHECK_SOLID(XU, YL, ZU, 0, CHUNK_SIZE_Y - 1, _z + 1)) shadow[3] += 0.1f;
								}
								if (isZl) {
									if (CHECK_SOLID(XU, __, ZL, 0, _y, _z - 1)) {
										shadow[0] += 0.1f;
										shadow[3] += 0.1f;
									}
								} else {
									if (CHECK_SOLID(XU, __, ZL, 0, _y, CHUNK_SIZE_Z - 1)) {
										shadow[0] += 0.1f;
										shadow[1] += 0.1f;
									}
								}
								if (isZu) {
									if (CHECK_SOLID(XU, __, ZU, 0, _y, _z + 1)) {
										shadow[1] += 0.1f;
										shadow[2] += 0.1f;
									}
								} else {
									if (CHECK_SOLID(XU, __, ZU, 0, _y, 0)) {
										shadow[0] += 0.1f;
										shadow[1] += 0.1f;
									}
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

#undef __
#undef XU
#undef XL
#undef YU
#undef YL
#undef ZU
#undef ZL
*/
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

void Chunk::Pair(Chunk& chunk){
	isRendering = false;
}

void Chunk::Render(Rendering::Shader& shader, Rendering::Texture& texture, const glm::mat4& MVP) const{
	if (isRendering)
		mesh.RenderMVP(shader, texture, MVP);
}

void Chunk::SaveToFile(const std::string& worldDir) const {
    std::string filename = GetChunkFileName(worldDir);
    std::ofstream file(filename, std::ios::binary);
    if (!file) return;
    
    file.write(reinterpret_cast<const char*>(blocks), sizeof(blocks));
}

bool Chunk::LoadFromFile(const std::string& worldDir) {
    std::string filename = GetChunkFileName(worldDir);
    std::ifstream file(filename, std::ios::binary);
    if (!file) return false;

    file.read(reinterpret_cast<char*>(blocks), sizeof(blocks));
    //std::cout << "Loaded chunk: " << x << " " << y << " " << z << "\n";
    return true;
}


std::string Chunk::GetChunkFileName(const std::string& worldDir) const {
	return worldDir + "/chunk_" + std::to_string(x) + "_" + std::to_string(y) + "_" + std::to_string(z) + ".bin";
}


bool CheckCollision(const Chunk& chunk, float min_x, float min_y, float min_z, float max_x, float max_y, float max_z){
	min_x -= CHUNK_SIZE_X * chunk.x;
	min_y -= CHUNK_SIZE_Y * chunk.y;
	min_z -= CHUNK_SIZE_Z * chunk.z;
	max_x -= CHUNK_SIZE_X * chunk.x;
	max_y -= CHUNK_SIZE_Y * chunk.y;
	max_z -= CHUNK_SIZE_Z * chunk.z;
	for (int _x = std::max(0.0f, std::round(min_x)); _x < std::min(static_cast<float>(CHUNK_SIZE_X), std::round(max_x) + 1); _x++){
		for (int _y = std::max(0.0f, std::round(min_y)); _y < std::min(static_cast<float>(CHUNK_SIZE_Y), std::round(max_y) + 1); _y++){
			for (int _z = std::max(0.0f, std::round(min_z)); _z < std::min(static_cast<float>(CHUNK_SIZE_Z), std::round(max_z) + 1); _z++){
				if (chunk.blocks[_x][_y][_z] != 0) return true;
			}
		}
	}
	return false;
}