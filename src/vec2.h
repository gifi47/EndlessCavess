/*
	Currently needed to support old code	
	TODO: replace this class with glm::vec2
*/

#ifndef GIFI47_VEC2_H
#define GIFI47_VEC2_H

namespace gifi47{

	struct vec2
	{
		float x;
		float y;
		vec2()
		{
			x = 0;
			y = 0;
		}
		vec2(float x, float y)
		{
			this->x = x;
			this->y = y;
		}
		vec2 operator+(const vec2& other)
		{
			return vec2(this-> x + other.x, this->y + other.y);
		}
		vec2 operator-(const vec2& other)
		{
			return vec2(this-> x - other.x, this->y - other.y);
		}
		vec2 operator*(const float& other)
		{
			return vec2(this-> x * other, this->y * other);
		}
		vec2 operator/(const float& other)
		{
			return vec2(this-> x / other, this->y / other);
		}
		bool operator==(const vec2& other)
		{
			return this->x == other.x && this->y == other.y;
		}
		static float distance(vec2 first, vec2 second)
		{
			return std::sqrt(std::pow(second.x - first.x, 2) + std::pow(second.y - first.y, 2));
		}
		static float distanceSqr(vec2 first, vec2 second)
		{
			return std::pow(second.x - first.x, 2) + std::pow(second.y - first.y, 2);
		}
		float length() const
		{
			return std::sqrt(std::pow(this->x, 2) + std::pow(this->y, 2));
		}
		vec2 normalize() const
		{
			float len = this->length();
			return vec2(this->x / len, this->y / len);
		}
		static float dot(vec2 first, vec2 second)
		{
			return first.x * second.x + first.y * second.y;
		}
		friend std::ostream& operator<<(std::ostream& os, const vec2& v)
    	{
        	os << "(" << v.x << ", " << v.y << ")";
        	return os;
    	}
	};
}

#endif //GIFI47_VEC2_H