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

Sphere::Sphere (v3 cen,
                const float r,
                const v3 col, 
                const float ref, 
                const float trans)
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
    float lo1 = inv_dir.y * (boxMin.y - r.orig.y);
    float hi1 = inv_dir.y * (boxMax.y - r.orig.y);

    t0 = max(min(lo, hi), min(lo1, hi1)); 
    t1 = min(max(lo, hi), max(lo1, hi1));

    float lo2 = inv_dir.z *(boxMin.z - r.orig.z);
    float hi2 = inv_dir.z *(boxMax.z - r.orig.z);
    t0  = max(t0, min(lo2, hi2));
    t1 = min(t1, max(lo2, hi2));

    return (t0 <= t1) && (t1 > 0);
}

void Box::normalize (const v3& pHit, v3& nHit) const
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
void Sphere::normalize (const v3& pHit, v3& nHit) const
{
    nHit = pHit - center_pos;
    glm::normalize(nHit);
}

STriangle::STriangle(v3 a, v3 b, v3 c, v3 norm, v3 col)
{
	A = a;
	B = b; 
	C = c;
	color = col;
	normal = norm;
}
bool STriangle::intersect(const SRay& ray, float &t0, float &t1) const
{
	v3 AB = B - A;
	v3 AC = C - A;
	v3 pvec = glm::cross(ray.dir, AC);
	float scalar = glm::dot(AB, pvec);

	if (fabs(scalar) < 1e-4f) // ray parallel to triangle
        return false;

	v3 tvec = ray.orig - A;
	float u = glm::dot(tvec,pvec) / scalar;
    if (u < 0 || u > 1) 
        return false;

	v3 qvec = glm::cross(tvec, AB);
	float v = glm::dot(ray.dir, qvec) / scalar;
	if (v < 0 || u + v > 1) 
        return false;

	t1 = t0 = glm::dot(AC,qvec) / scalar;

	return true; 
} 

void STriangle::normalize(const v3& pHit, v3& nHit) const
{
	 nHit = glm::normalize(pHit);
}

SRouter::SRouter(v3 Position,
                 float Radius, 
                 v3 Color,
                 float signal_strength)
{
    sig_strength = signal_strength;
    Sphere wifi(Position, Radius, Color, 0.f, 0.f);
}

void SRouter::fill_grid(SVoxelGrid& grid, uint num)
{
	for(uint i=0; i < num ; i++){
		//construct random ray
		float x = -1.0f + static_cast <float> (rand()) / static_cast <float> (RAND_MAX/(2.0f));
		float y = -1.0f + static_cast <float> (rand()) / static_cast <float> (RAND_MAX/(2.0f));
		float z = -1.0f + static_cast <float> (rand()) / static_cast <float> (RAND_MAX/(2.0f));
		//cout << x << " " << y << " " << z << endl;
		v3 direction = glm::normalize(v3(x,y,z));
		SRay random_ray = SRay(center_pos, direction);
		march(random_ray,grid, 0.f, 100.f);
	}
}

float SRouter::power(v3 point)
{
	float distance = glm::length(point - center_pos);
	if(distance < radius)
		return sig_strength;
	else
		return std::min(sig_strength, sig_strength/(distance*distance));
}

void SRouter::march(SRay& ray,SVoxelGrid& grid, float start, float end){
	float depth = start;
	float step = 0.05; // 1cm
	for(int i=0; i < STEPS; i++){
		v3 point = ray.orig + v3(ray.dir.x*depth,ray.dir.y*depth,ray.dir.z*depth);
		depth+=step;

		if(depth > end){
			return;
		}

		int index = grid.find(point);
		if(index > 0){
			if(grid.voxels[index].value < power(point))
			   grid.voxels[index].value = power(point);
		}
		else{
			// out of grid
			return;
		}
		
	}
	return;
}