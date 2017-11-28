#pragma once
#include "types.h"

struct Figure //abstract 
{
    v3 color;
    float reflect, transparent;
    virtual bool intersect ( const SRay&, float&, float&) const = 0;
    virtual void normalize ( const v3&, v3&) const = 0;

    virtual ~Figure(){}
};

struct Box : Figure
{
    v3 boxMin, boxMax;
    Box(const v3 vmin, const v3 vmax, const v3 col, const float ref, const float trans);

    bool intersect (const SRay& r, float& t0, float& t1) const; 
    void normalize (const v3& pHit, v3& nHit) const;
};

struct Sphere : Figure
{   
    v3 center_pos;
    float radius, radius2;   

    Sphere (v3 cen = v3(0.f, 0.f, 0.f),
            const float r = 1.f,
            const v3 col = v3(1.f, 0.f, 0.f), 
            const float ref = 0.f, 
            const float trans = 0.f);
    
    bool intersect (const SRay& r, float& t0, float& t1) const; 
    void normalize (const v3& pHit, v3& nHit) const;
};

struct STriangle: Figure
{
	v3 A, B, C;
	v3 normal;
	STriangle(v3 a, v3 b, v3 c, 
              v3 norm = v3(0.f, 0.f, 0.f),
              v3 col = v3(1.f, 1.f, 1.f));
	
	bool intersect (const SRay& r, float& t0, float& t1) const; 
    void normalize (const v3& pHit, v3& nHit) const;
};

struct SRouter : public Sphere 
{
    float sig_strength;
 	
    SRouter(v3 Position = v3(0.f, 0.f, 0.f),
            float Radius = 1.f, 
            v3 Color = v3(1.f, 0.f, 0.f),
            float signal_strength = 10000);
    
	float power(v3 point);
	void fill_grid(std::vector<Figure*> figures, SVoxelGrid& grid, uint num);
	void march(SRay& ray, std::vector<Figure*> figures, SVoxelGrid& grid, int recursion_step);
	bool check_intersect(const SRay& ray, std::vector<Figure*> figures, float& t, size_t& index);
};


// struct SRouter : public Box 
// {
//     float signal_strength;
 	 
//     SRouter(v3 pos = v3(0.f, 0.f, 0.f),
//             float width = 1.f, 
//             float heigth = 1.f, 
//             v3 color = v3(1.f, 0.f, 0.f),
//             float ss = 10000);
// };