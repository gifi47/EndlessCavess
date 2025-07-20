#include "noise.h"

float noise::clamp(float x, float lowerlimit, float upperlimit) {
  	if (x < lowerlimit) return lowerlimit;
  	if (x > upperlimit) return upperlimit;
  	return x;
}

float noise::smoothstep(float edge0, float edge1, float x) {
	// Scale, and clamp x to 0..1 range
	x = noise::clamp((x - edge0) / (edge1 - edge0));

	return x * x * (3.0f - 2.0f * x);
}

float noise::smooth(float x, float f1, float f2){
	return noise::smoothstep(0, 1, x) * (f2 - f1) + f1;
}


noise::SimpleNoise::SimpleNoise(int width, int height, long long seed){
	this->width = width;
	this->height = height;

	std::random_device rd;
    std::mt19937 gen(rd());
    
    // Uniform real distribution
    std::uniform_real_distribution<> real_dis(0.0, 1.0);

	grid.resize(width);
	for (int w = 0; w < width; w++){
		grid[w] = std::vector<float>(height);
		for (int h = 0; h < height; h++){
			grid[w][h] = real_dis(gen);
		}
	}
}

float noise::SimpleNoise::GetValue(float x, float y){
	float w_f = x * (width - 1);
	int w_i = static_cast<int>(w_f);
	if (w_i > width - 2) w_i = width - 2;
	else if (w_i < 0) w_i = 0;

	float h_f = y * (height - 1);
	int h_i = static_cast<int>(h_f);
	if (h_i > height - 2) h_i = height - 2;
	else if (h_i < 0) h_i = 0;

	float h_y = 1 - ((h_i + 1) - h_f);
	float w_x = 1 - ((w_i + 1) - w_f);
	//return (grid[w_i][h_i] * h_y + grid[w_i][h_i + 1] * (1 - h_y)) * w_x + (grid[w_i + 1][h_i] * h_y + grid[w_i + 1][h_i + 1] * (1 - h_y)) * (1 - w_x);
	return noise::smooth(w_x, noise::smooth(h_y, grid[w_i][h_i], grid[w_i][h_i + 1]), noise::smooth(h_y, grid[w_i + 1][h_i], grid[w_i + 1][h_i + 1]));
}

/*template <typename T>
void noise::SimpleNoise::Print(T& str){
	for (int h = 0; h < height; h++){
		for (int w = 0; w < width; w++){
			str << grid[w][h] << " ";
		}
		str << "\n";
	}
}*/

noise::SimpleNoise3D::SimpleNoise3D(int width, int height, int depth, long long seed){
	this->width = width;
	this->height = height;
	this->depth = depth;

	std::random_device rd;
    std::mt19937 gen(rd());
    
    // Uniform real distribution
    std::uniform_real_distribution<> real_dis(0.0, 1.0);

	grid.resize(width);
	for (int w = 0; w < width; w++){
		grid[w] = std::vector<std::vector<float> >(height);
		for (int h = 0; h < height; h++){
			grid[w][h] = std::vector<float>(depth);
			for (int d = 0; d < depth; d++){
				grid[w][h][d] = real_dis(gen);
			}
		}
	}
}

float noise::SimpleNoise3D::GetValue(float x, float y, float z){
	float w_f = x * (width - 1);
	int w_i = static_cast<int>(w_f);
	if (w_i > width - 2) w_i = width - 2;
	else if (w_i < 0) w_i = 0;

	float h_f = y * (height - 1);
	int h_i = static_cast<int>(h_f);
	if (h_i > height - 2) h_i = height - 2;
	else if (h_i < 0) h_i = 0;

	float d_f = z * (depth - 1);
	int d_i = static_cast<int>(d_f);
	if (d_i > depth - 2) d_i = depth - 2;
	else if (d_i < 0) d_i = 0;

	float h_y = 1 - ((h_i + 1) - h_f);
	float w_x = 1 - ((w_i + 1) - w_f);
	float d_z = 1 - ((d_i + 1) - d_f);

	float z1 = noise::smooth(d_z, grid[w_i][h_i][d_i], 		grid[w_i][h_i][d_i + 1]);
	float z2 = noise::smooth(d_z, grid[w_i][h_i + 1][d_i], 	grid[w_i][h_i + 1][d_i + 1]);
	float z3 = noise::smooth(d_z, grid[w_i + 1][h_i][d_i], 	grid[w_i + 1][h_i][d_i + 1]);
	float z4 = noise::smooth(d_z, grid[w_i + 1][h_i + 1][d_i], grid[w_i + 1][h_i + 1][d_i + 1]);

	return noise::smooth(w_x, noise::smooth(h_y, z1, z2), noise::smooth(h_y, z3, z4));
}