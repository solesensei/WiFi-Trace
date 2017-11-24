#pragma once

#include <vector>
#include <cmath>
#include <tuple>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "matrix.h"
#include "EasyBMP.h"


#define PI 3.1415926535897932384626433832795028841971

typedef Matrix<std::tuple<uint, uint, uint>> Image;

typedef glm::vec3 v3;
typedef glm::vec2 v2;
typedef glm::uvec2 uv2;
typedef glm::uvec3 uv3;
typedef glm::mat4 m4;

struct SCamera
{
	float fov;
	v3 pos;  // Camera position and orientation
	v3 front;  // Orthonormal basis
	v3 right;
	v3 up;

	v2 viewAngle;    // View angles, rad
   	uv2 resolution;  // Image resolution: w, h

	std::vector<v3> v_pixels;  // Pixel array

	SCamera(v3 Pos 	  = v3(0.0f, 0.0f, 2.0f),
		    v3 Up 	  = v3(0.0f, 1.0f, 0.0f),
		   	v3 Front  = v3(0.0f, 0.0f,-1.0f),
		   	v3 Right  = v3(1.0f, 0.0f, 0.0f),
		    float FOV = PI/2
		   ){}

	m4 GetViewMatrix() const{
		return glm::lookAt(pos, pos+front, up);
	}

};

struct SMesh
{
  std::vector<v3> m_vertices;  // vertex positions
  std::vector<uv3> m_triangles;  // vetrex indices
};

struct SRay
{
    v3 orig;
    v3 dir;
    
    SRay(const v3& origin, const v3& direction): orig(origin), dir(direction)
    {}
    SRay (): orig(v3()), dir(v3())
    {}
    
    static SRay build_ray(float x, 
                         float y, 
                         float width, 
                         float height);
    // {
        // float fov = PI/(2.0f);
        // v3 ray_dir;
        // ray_dir.x_ = x+0.5 - (w/2.0f);
        // ray_dir.y_ = y+0.5 - (h/2.0f);
        // ray_dir.z_ = -(w)/tan(fov/2.0f);
        // return Ray( v3(0) , ray_dir.normalize() );
    // }
    // vec3& normalize() 
    // { 
    //     float nor2 = length2(); 
    //     if (nor2 > 0)
    //     { 
    //         float invNor = 1 / sqrt( nor2 ); 
    //         x_*= invNor, y_*= invNor, z_*= invNor; 
    //     } 
    //     return *this; 
    // } 

};

struct SColor 
{
    float r, g, b;
    SColor(float x = 0, float y = 0, float z = 0): r(x), g(y), b(z) {}

    SColor operator * (const float &a) const  { return SColor(r * a, g * a, b * a); }
    SColor operator * (const SColor &v) const { return SColor(r * v.r, g * v.g, b * v.b); }
    SColor operator + (const SColor &v) const { return SColor(r + v.r, g + v.g, b + v.b); }
};

struct SLight 
{
    v3 PT;
    v3 dir;
    float intens;
    SLight(const v3& pos, const float i = 1): PT(pos), intens(i) {}
    
};
