#pragma once
#include "types.h"

struct Figure //abstract 
{
    SColor color;
    float reflect, transparent;
    virtual bool intersect  ( const SRay&, float&, float&) const = 0;
    virtual void getNorm ( const v3&, v3&) const = 0;

    virtual ~Figure(){}
};

struct Box : Figure
{
    v3 boxMin,boxMax;
    Box(const v3& vmin, const v3& vmax, 
        const SColor col, const float ref, const float trans)
    {
        boxMin = vmin;
        boxMax = vmax;
        color = col;
        reflect = ref;
        transparent = trans;
    }

    bool intersect (const SRay& r, float& t0, float& t1) const; 
    void getNorm (const v3& pHit, v3& nHit) const;
};

struct Sphere : Figure
{   
    v3 center_pos;
    float radius, radius2;   

    Sphere()
    {
        transparent = reflect = 0.f;
        center_pos = v3();
        radius = 100;
        radius2 = 100*100;
    }
    Sphere (v3 cen = v3(0.f, 0.f, 0.f),
            const float r = 1.f,
            const SColor col = SColor(), 
            const float ref = 0.f, 
            const float trans = 0.f)
    {
        center_pos = cen; 
        radius = r; 
        radius2 = r*r; 
        color = col, 
        reflect = ref;
        transparent = trans;
    }
    
    bool intersect (const SRay& r, float& t0, float& t1) const; 
    void getNorm(const v3& pHit, v3& nHit) const;
};


struct SRouter : public Sphere 
{
    float Signal_Strength;
 	
    SRouter(v3 point = v3(0.f, 0.f, 0.f),
            float radius = 1.f, 
            float strength = 10000);
};