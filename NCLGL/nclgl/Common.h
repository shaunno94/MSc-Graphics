#pragma once
#include <string>

static const float		PI = 3.14159265358979323846f;	
static const float		PI_OVER_360 = PI / 360.0f;
static const float		RAD_TO_DEG_CONV = (180.0f / PI);
static const float		DEG_TO_RAD_CONV = (PI / 180.0f);

//Radians to degrees
static inline double RadToDeg(const float deg)	
{
	return deg * RAD_TO_DEG_CONV;
};

//Degrees to radians
static inline double DegToRad(const float rad)	
{
	return rad * DEG_TO_RAD_CONV;
};

#define max(a,b)    (((a) > (b)) ? (a) : (b))
#define min(a,b)    (((a) < (b)) ? (a) : (b))

struct File_Locs
{
	static std::string SHADER_DIR;
	static std::string TEXTURE_DIR;
	static std::string MESH_DIR;
};