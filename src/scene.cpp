#include <iostream>
// #include <fstream>
// #include <cstdlib>
#include <cmath>
#include "geometry.h"

using std::cerr;
using std::endl;

float max(const float a, const float b)
{
    return a > b ? a : b;
}

float min(const float a, const float b)
{
    return a < b ? a : b;
}

float mix(const float &a, const float &b, const float &mix)
{
        return b * mix + a * (1 - mix);
}

bool QuadEq(const float &a, const float &b, const float &c, float &x0, float &x1) 
{ 
    if (a - 0.f < 0.0001) cerr << "QuadEq :: not equation" << endl;
    float discr = b * b - 4 * a * c; 
    if (discr < 0) return false; 
    else if (discr - 0.f < 0.0001) { 
        x0 = x1 = - 0.5 * b / a; 
    } 
    else { 
        x0 = 0.5 * (-b - sqrt(discr)) / a; 
        x1 = 0.5 * (-b + sqrt(discr)) / a; 
    } 
    return true; 
} 

bool Box::intersect (const SRay& r, float& t0, float& t1) const 
{
    // v3 inv_dir = 1/r.dir;
    // float lo = inv_dir.x_*(boxMin.x_ - r.orig.x_);
    // float hi = inv_dir.x_*(boxMax.x_ - r.orig.x_);
    // t0  = min(lo, hi);
    // t1 = max(lo, hi);

    // float lo1 = inv_dir.y_*(boxMin.y_ - r.orig.y_);
    // float hi1 = inv_dir.y_*(boxMax.y_ - r.orig.y_);
    // t0 = max(t0, min(lo1, hi1));
    // t1 = min(t1, max(lo1, hi1));

    // float lo2 = inv_dir.z_*(boxMin.z_ - r.orig.z_);
    // float hi2 = inv_dir.z_*(boxMax.z_ - r.orig.z_);
    // t0  = max(t0, min(lo2, hi2));
    // t1 = min(t1, max(lo2, hi2));

    // return (t0 <= t1) && (t1 > 0);
    return false;
}

void Box::getNorm (const v3& pHit, v3& nHit) const
{
    // nHit = v3(0);
    // float eps = 1e-7;
    // if (      fabs( pHit.x_ - boxMax.x_ ) < eps ) //righ
    //     nHit.x_ = 1;
    // else if ( fabs( pHit.x_ - boxMin.x_ ) < eps ) //left
    //     nHit.x_ = -1;
    // else if ( fabs( pHit.y_ - boxMax.y_ ) < eps ) //down
    //     nHit.y_ = 1;
    // else if ( fabs( pHit.y_ - boxMin.y_ ) < eps ) //up
    //     nHit.y_ = -1;
    // else if ( fabs( pHit.z_ - boxMax.z_ ) < eps ) //back
    //     nHit.z_ = -1;
    // else if ( fabs( pHit.z_ - boxMin.z_ ) < eps ) //front
    //     nHit.z_ = 1;
    // else { cerr << "getNorm-box" << endl; } 
}

bool Sphere::intersect (const SRay& r, float& t0, float& t1) const 
{   
    // v3 v = center - r.orig;
    // float tca = v.dot(r.dir);
    // if (tca < 0) return false;
    // float d2 = v.dot(v) - tca*tca;
    // if(d2 > radius2) return false;
    // float thc = sqrt ( radius2-d2 );
    // t0 = tca - thc;
    // t1 = tca + thc;

    return false;
}
void Sphere::getNorm(const v3& pHit, v3& nHit) const
{
    // nHit = pHit - center;
    // nHit.normalize();
}
