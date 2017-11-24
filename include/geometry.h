#pragma once
#include "types.h"

struct Figure //abstract 
{
    v3 color;
    float reflect, transparent;
    virtual bool intersect  ( const SRay&, float&, float&) const = 0;
    virtual void getNorm ( const v3&, v3&) const = 0;

    virtual ~Figure(){}
};

struct Box : Figure
{
    v3 boxMin, boxMax;
    Box(const v3 vmin, const v3 vmax, const v3 col, const float ref, const float trans);

    bool intersect (const SRay& r, float& t0, float& t1) const; 
    void getNorm (const v3& pHit, v3& nHit) const;
};

struct Sphere : Figure
{   
    v3 center_pos;
    float radius, radius2;   

    Sphere (v3 cen,
            const float r,
            const v3 col, 
            const float ref, 
            const float trans);
    
    bool intersect (const SRay& r, float& t0, float& t1) const; 
    void getNorm(const v3& pHit, v3& nHit) const;
};


struct SRouter : public Sphere 
{
    float sig_strength;
 	
    SRouter(v3 pos = v3(0.f, 0.f, 0.f),
            float radius = 1.f, 
            float signal_strength = 10000);
};

// struct SRouter : public Box 
// {
//     float signal_strength;
 	
//     SRouter(v3 pos = v3(0.f, 0.f, 0.f),
//             float width = 1.f, 
//             float heigth = 1.f, 
//             float ss = 10000);
// };