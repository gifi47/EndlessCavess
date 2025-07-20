/*
	Makeshift noise implementation
	Should've replaced with proper noise lib
*/
#ifndef NOISE_H
#define NOISE_H

#include <vector>
#include <random>
#include "glm/glm.hpp"

namespace noise {

	float clamp(float x, float lowerlimit = 0.0f, float upperlimit = 1.0f);

	float smoothstep(float edge0, float edge1, float x);

	float smooth(float x, float f1, float f2);

	class SimpleNoise{
	public:
		int width;
		int height;

		std::vector<std::vector<float> > grid;

		SimpleNoise(int width, int height, long long seed);

		float GetValue(float x, float y);

		template <typename T>
		void Print(T& str){
			for (int h = 0; h < height; h++){
				for (int w = 0; w < width; w++){
					str << grid[w][h] << " ";
				}
				str << "\n";
			}
		}

	};

	class SimpleNoise3D{
	public:
		int width;
		int height;
		int depth;

		std::vector<std::vector<std::vector<float> > > grid;

		SimpleNoise3D(int width, int height, int depth, long long seed);

		float GetValue(float x, float y, float z);

	};

}

#endif //NOISE_H