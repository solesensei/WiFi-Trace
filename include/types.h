#pragma once

#include <vector>
#include <string>
#include <cmath>
#include <tuple>
#include <assert.h>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <limits>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "matrix.h"
#include "EasyBMP.h"

#define PI 3.1415926535897932384626433832795028841971
#define MARCH_STEPS 2000
#define REC_STEPS 3

#define EPS 1e-7f

typedef Matrix<std::tuple<uint, uint, uint>> Image;

typedef glm::vec3 vec3;
typedef glm::vec2 vec2;
typedef glm::uvec2 uvec2;
typedef glm::uvec3 uvec3;
typedef glm::mat4 mat4;

// ============
using std::cerr;
using std::endl;
// ============

struct SCamera
{
	vec3 pos;  // Camera position and orientation
	vec3 up;
	vec3 right;
	vec3 front;  // Orthonormal basis = cross [up, right]
	float fov; // Field of view, angle
 
	float width, height; // Image resolution

	SCamera(vec3 Pos 	  = vec3(0.f, 0.f, 2.f),
		    vec3 Up 	  = vec3(0.f, 1.f, 0.f),
		   	vec3 Right  = vec3(-1.f, 0.f, 0.f),
		    float FOV = PI/2
		   );
};

struct SRay
{
    vec3 orig; // Cast point
    vec3 dir; // Cast direction
    float strength;
	float refl; // Reflection coefficient

    SRay(const vec3& origin, const vec3& direction, float s = 0.f, float reflect = 0.f):
		orig(origin), dir(direction), strength(s), refl(reflect)
    {}
    SRay (): 
		orig(vec3()), dir(vec3()), strength(0), refl(0)
    {}
    void set_strength(float s);
    static SRay build_ray(float x, float y, const SCamera& camera);
};

struct SLight 
{
    vec3 pos; // Light position
    vec3 intens; // Light intensity
    SLight(const vec3& p, const float i = 1): pos(p), intens(vec3(i,i,i))
	{}

};

struct SPhong
{
	static vec3 phong_calc(const vec3& ambi_const, // ambient reflection constant
						   const vec3& diff_const, // diffuse reflection constant
				           const vec3& spec_const, // specular reflection constant
				           float alpha, // shininess constant for this material
				           const vec3& normal, // normal at hit point on the surface
				           const vec3& hit,
				           const SCamera& cam,
				           SLight light
				           ){
            // direction from the hit point to toward light source
		vec3 dir_light = glm::normalize(light.pos - hit);
            // direction pointing towards the viewer
		vec3 dir_view = glm::normalize(cam.pos - hit);
            // direction that a perfectly reflected ray of light would take from this point on the surface
            // glm::reflect(a,b) : return a - 2*(a,b)*b
            // dir_reflect = 2*(dir_light,normal)*normal - dir_light
		vec3 dir_reflect = glm::normalize(-glm::reflect(dir_light, normal));  		
        
        float dot_light_normal = glm::dot(dir_light, normal); 
		float dot_reflect_view = glm::dot(dir_reflect, dir_view); 
		
        float radius = glm::distance(light.pos, hit);
        float area = 4*PI*radius*radius*1e-5f;
	    vec3 color = light.intens / area;

		if(dot_light_normal < 0.f) // no light
			return vec3(0.f, 0.f, 0.f);

            // return diffuse 
		if(dot_reflect_view < 0.f) // no reflections 
			return color * ( ambi_const + diff_const * dot_light_normal );

		    // return ambient + diffuse + specular
		float specular = pow(dot_reflect_view, alpha);
		return color * ( ambi_const + diff_const*dot_light_normal + spec_const*specular );
	}
};

struct SVoxel
{
 	vec3 pos;
 	float side;
 	vec3 color;
 	float value;

 	SVoxel();
 	SVoxel(vec3 Position,
          float Side,
          vec3 Color = vec3(1.f, 1.f, 1.f),
          float Value = 0.f);
 	
    float calc_SDF(vec3 point); // Signed Distance Function calculating
 	float calc_depth(SRay& ray, float start, float end); // get depth via SDF calculating
};

struct SVoxelGrid
{
	std::vector<SVoxel> voxels;
	vec3 topleft; // Size of grid
	vec3 botright;
	float voxel_side; 
	uint num_x, num_y, num_z; // Number of voxels at each axis

	SVoxelGrid(vec3 Topleft, vec3 Botright);
	
    void initialize(); // Filling voxels vector
	int find(vec3 point); // Get voxel via point
	void filter(); // Box filter, reduce noises
	float neighbourBOX(const vec3&);
	void print(); // Print all voxels values
};