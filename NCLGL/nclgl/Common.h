/*
Author: Shaun Heald
Common helper functions / variables and file directories.
*/

#pragma once
#include <string>

#define	PI 3.14159265358979323846
#define	PI_OVER_360 PI / 360.0
#define	RAD_TO_DEG_CONV (180.0 / PI)
#define	DEG_TO_RAD_CONV (PI / 180.0)

#define max(a,b)    (((a) > (b)) ? (a) : (b))
#define min(a,b)    (((a) < (b)) ? (a) : (b))

//Radians to degrees
static inline double RadToDeg(const float rad)	
{
	return rad * RAD_TO_DEG_CONV;
};

//Degrees to radians
static inline double DegToRad(const float deg)	
{
	return deg * DEG_TO_RAD_CONV;
};

//Top file directory location for shaders, meshes and textures
struct File_Locs
{
	static std::string SHADER_DIR;
	static std::string TEXTURE_DIR;
	static std::string MESH_DIR;
};