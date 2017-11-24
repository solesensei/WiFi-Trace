#include "geometry.h"
using std::max;
using std::min;
using std::cerr;
using std::endl;
using std::sqrt;
using std::fabs;


Box::Box(const v3 vmin, const v3 vmax, const v3 col, const float ref, const float trans)
{
    boxMin = vmin;
    boxMax = vmax;
    color = col;
    reflect = ref;
    transparent = trans;
}

Sphere::Sphere (v3 cen = v3(0.f, 0.f, 0.f),
                const float r = 1.f,
                const v3 col = v3(0.f, 0.f, 0.f), 
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


bool QuadEq(const float &a, const float &b, const float &c, float &x0, float &x1) 
{ 
    if (a - 0.f < 0.0001f) cerr << "QuadEq :: not equation" << endl;
    
    float discriminant = b*b - 4*a*c; 
    if (discriminant < 0) 
        return false; 
    else if (discriminant - 0.f < 0.0001f) 
        x0 = x1 = -0.5 * b / a; 
    else 
    { 
        x0 = 0.5 * (-b - sqrt(discriminant)) / a; 
        x1 = 0.5 * (-b + sqrt(discriminant)) / a; 
    } 
    return true; 
} 

bool Box::intersect (const SRay& r, float& t0, float& t1) const 
{
    v3 inv_dir = 1.f/r.dir;
    float lo = inv_dir.x * (boxMin.x - r.orig.x);
    float hi = inv_dir.x * (boxMax.x - r.orig.x);
    t0  = min(lo, hi);
    t1 = max(lo, hi);

    float lo1 = inv_dir.y * (boxMin.y - r.orig.y);
    float hi1 = inv_dir.y * (boxMax.y - r.orig.y);
    t0 = max(t0, min(lo1, hi1)); 
    t1 = min(t1, max(lo1, hi1));

    float lo2 = inv_dir.z *(boxMin.z - r.orig.z);
    float hi2 = inv_dir.z *(boxMax.z - r.orig.z);
    t0  = max(t0, min(lo2, hi2));
    t1 = min(t1, max(lo2, hi2));

    return (t0 <= t1) && (t1 > 0);
}

void Box::getNorm (const v3& pHit, v3& nHit) const
{
    nHit = v3(0);
    float eps = 1e-7;
    if      ( fabs( pHit.x - boxMax.x ) < eps ) //righ
        nHit.x = 1;
    else if ( fabs( pHit.x - boxMin.x ) < eps ) //left
        nHit.x = -1;
    else if ( fabs( pHit.y - boxMax.y ) < eps ) //down
        nHit.y = 1;
    else if ( fabs( pHit.y - boxMin.y ) < eps ) //up
        nHit.y = -1;
    else if ( fabs( pHit.z - boxMax.z ) < eps ) //back
        nHit.z = -1;
    else if ( fabs( pHit.z - boxMin.z ) < eps ) //front
        nHit.z = 1;
    else { cerr << "getNorm-box" << endl; } 
}

bool Sphere::intersect (const SRay& r, float& t0, float& t1) const 
{   
    v3 v = center_pos - r.orig;
    float tca = glm::dot(v, r.dir);
    if (tca < 0) return false;
    
    float d2 = glm::dot(v, v) - tca*tca;
    if(d2 > radius2) return false;
    
    float thc = sqrt ( radius2-d2 );
    t0 = tca - thc;
    t1 = tca + thc;
    return true;
}
void Sphere::getNorm(const v3& pHit, v3& nHit) const
{
    nHit = pHit - center_pos;
    glm::normalize(nHit);
}
