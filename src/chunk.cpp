#include "chunk.h"

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