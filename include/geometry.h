#pragma once
#include "types.h"

struct Figure // abstract 
{
    vec3 color;
    float reflect, transparent; // factors
    virtual bool intersect ( const SRay&, float&, float&) const = 0;
    virtual vec3 normalize ( const vec3& ) const = 0;

    virtual ~Figure(){}
};

struct Box : public Figure
{
    vec3 boxMin, boxMax;
    Box(const vec3 vmin, const vec3 vmax, const vec3 col, const float ref, const float trans);

    bool intersect (const SRay& r, float& t0, float& t1) const; 
    vec3 normalize (const vec3& pHit) const;
};

struct Sphere : public Figure
{   
    vec3 center_pos;
    float radius, radius2;   

    Sphere (vec3 cen = vec3(0.f, 0.f, 0.f),
            const float r = 1.f,
            const vec3 col = vec3(1.f, 0.f, 0.f), 
            const float ref = 0.f, 
            const float trans = 0.f);
    
    bool intersect (const SRay& r, float& t0, float& t1) const; 
    vec3 normalize (const vec3& pHit) const;
};

struct STriangle : public Figure
{
	vec3 A, B, C;
	vec3 normal;
	STriangle(vec3 a, vec3 b, vec3 c, 
              vec3 norm = vec3(0.f, 0.f, 0.f),
              vec3 col = vec3(1.f, 1.f, 1.f));
	
	bool intersect (const SRay& r, float& t0, float& t1) const; 
    vec3 normalize (const vec3& pHit) const;
};

struct SRouter : public Sphere 
{
    float sig_strength; // Strength of the signal that router cast
 	
    SRouter(vec3 Position = vec3(0.f, 0.f, 0.f),
            float Radius = 1.f, 
            vec3 Color = vec3(1.f, 0.f, 0.f),
            float signal_strength = 10000);
    
        // Calculate signal strength via distance point 
	float calc_strength(vec3 point); 
        // Fill voxel grid via marching random rays
	void fill_grid(std::vector<Figure*> figures, SVoxelGrid& grid, uint num);
        // Ray marching algorithm 
	void march(SRay& ray, std::vector<Figure*> figures, SVoxelGrid& grid, int recursion_step);
        // Check ray and objects intersection
	bool check_intersect(const SRay& ray, std::vector<Figure*> figures, float& t, size_t& index);
};


// struct SRouter : public Box 
// {
//     float signal_strength;
//     SRouter(vec3 pos = vec3(0.f, 0.f, 0.f),
//             float width = 1.f, 
//             float heigth = 1.f, 
//             vec3 color = vec3(1.f, 0.f, 0.f),
//             float ss = 10000);
// };