#include "geometry.h"
using std::max;
using std::min;
using std::cerr;
using std::endl;
using std::sqrt;
using std::fabs;


Box::Box(const vec3 vmin, const vec3 vmax, const vec3 col, const float ref, const float trans)
{
    boxMin = vmin;
    boxMax = vmax;
    color = col;
    reflect = ref;
    transparent = trans;
}

Sphere::Sphere (vec3 cen,
                const float r,
                const vec3 col, 
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
    vec3 inv_dir = 1.f/r.dir;
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

vec3 Box::normalize (const vec3& pHit) const
{
    vec3 nHit = vec3(0);
    if      ( fabs( pHit.x - boxMax.x ) < EPS ) //right
        nHit.x = 1;
    else if ( fabs( pHit.x - boxMin.x ) < EPS ) //left
        nHit.x = -1;
    else if ( fabs( pHit.y - boxMax.y ) < EPS ) //down
        nHit.y = 1;
    else if ( fabs( pHit.y - boxMin.y ) < EPS ) //up
        nHit.y = -1;
    else if ( fabs( pHit.z - boxMax.z ) < EPS ) //back
        nHit.z = -1;
    else if ( fabs( pHit.z - boxMin.z ) < EPS ) //front
        nHit.z = 1;
    return nHit;
}

bool Sphere::intersect (const SRay& r, float& t0, float& t1) const 
{   
    vec3 v = center_pos - r.orig;
    float tca = glm::dot(v, r.dir);
    if (tca < 0) return false;
    
    float d2 = glm::dot(v, v) - tca*tca;
    if(d2 > radius2) return false;
    
    float thc = sqrt ( radius2-d2 );
    t0 = tca - thc;
    t1 = tca + thc;
    return true;
}
vec3 Sphere::normalize (const vec3& pHit) const
{
    vec3 nHit = pHit - center_pos;
    return glm::normalize(nHit);
}

STriangle::STriangle(vec3 a, vec3 b, vec3 c, vec3 norm, vec3 col)
{
	A = a;
	B = b; 
	C = c;
	color = col;
	normal = norm;
}
bool STriangle::intersect(const SRay& ray, float &t0, float &t1) const
{
	vec3 AB = B - A;
	vec3 AC = C - A;
	vec3 pvec = glm::cross(ray.dir, AC);
	float scalar = glm::dot(AB, pvec);

	if (fabs(scalar) < 1e-4f) // ray parallel to triangle
        return false;

	vec3 tvec = ray.orig - A;
	float u = glm::dot(tvec,pvec) / scalar;
    if (u < 0 || u > 1) 
        return false;

	vec3 qvec = glm::cross(tvec, AB);
	float v = glm::dot(ray.dir, qvec) / scalar;
	if (v < 0 || u + v > 1) 
        return false;

	t1 = t0 = glm::dot(AC,qvec) / scalar;

	return true; 
} 

vec3 STriangle::normalize(const vec3& pHit) const
{
	 return glm::normalize(normal);
}

SRouter::SRouter(vec3 Position,
                 float Radius, 
                 vec3 Color,
                 float signal_strength):
    Sphere(Position, Radius, Color)
{
    sig_strength = signal_strength;
}

void SRouter::fill_grid(std::vector<Figure*> figures, SVoxelGrid& grid, uint num)
{
		// build random ray
    uint i;
    #pragma omp parallel for private(i)
	for(i = 0; i < num ; ++i)
    {
        float x = rand() / (RAND_MAX/(2.f)) - 1;
        float y = rand() / (RAND_MAX/(2.f)) - 1;
        float z = rand() / (RAND_MAX/(2.f)) - 1;

        vec3 dir = glm::normalize(vec3(x,y,z));
		SRay random_ray = SRay(center_pos, dir, sig_strength);
		march(random_ray, figures, grid, 0);
	}
}

float SRouter::calc_strength(vec3 point)
{
	float distance = glm::length(point - center_pos);
	if(distance < radius)
		return 0.f;
	else
		return min(sig_strength, sig_strength/(distance*distance));
}

void SRouter::march(SRay& ray, std::vector<Figure*> figures, SVoxelGrid& grid, int r_step)
{
	float depth = 0.f;
	float d_step = 0.02f;
    float end = 100.f;
    float t_close = 100.f;
    size_t index;
    if (check_intersect(ray, figures, t_close, index)){
        
        end = t_close;
        vec3 pHit = ray.orig + ray.dir*t_close;
        float refl_chance =  float(rand()) / float(RAND_MAX);
        if (refl_chance > 0.5f){ // reflection
            if(r_step < REC_STEPS){
                vec3 nHit = figures[index]->normalize(pHit);
                pHit += nHit*0.01f;
                nHit = figures[index]->normalize(pHit);        
                vec3 refl_dir = glm::normalize(glm::reflect(ray.dir, nHit));
                float s = calc_strength(pHit);
                SRay refl_ray(pHit, refl_dir, s);
                march(refl_ray, figures, grid, r_step+1);
            }
        }else{ // absorption
            vec3 nHit =figures[index]->normalize(pHit);
            pHit -= nHit*0.01f;
            float s = calc_strength(pHit) - ray.refl*10.f;
            SRay tray(pHit, ray.dir, s);
            float t_near = 100.f;    
            size_t idx;
            if(check_intersect(tray, figures, t_near, idx)){
                vec3 p = tray.orig + tray.dir*t_near;
                vec3 n = figures[idx]->normalize(p);
                p += n*0.01f;
                SRay no_refl_ray(p, tray.dir, tray.strength, tray.refl+1);
                march(no_refl_ray, figures, grid, r_step); 
            }
        }
    }
	for(int i = 0; i < MARCH_STEPS; ++i){
		vec3 point = ray.orig + vec3(ray.dir.x*depth, ray.dir.y*depth, ray.dir.z*depth);
		depth += d_step;
        ray.strength = calc_strength(point) - ray.refl*10.f;
		if(depth > end){
			return;
		}
            // set grid voxel with signal strength
		int idx = grid.find(point);
		if(idx > 0){
			if(grid.voxels[idx].value < ray.strength)
			   grid.voxels[idx].value = ray.strength;
		}
		else{
			// out of grid
			return;
		}
		
	}
	return;
}

bool SRouter::check_intersect(const SRay& ray,std::vector<Figure*> figures, float& t_close, size_t& i_close)
{
    bool isInter = false;
	float t0 = 100.f;
    float t1 = t0;
	for(size_t i = 0; i < figures.size(); ++i)
    {
			if(figures[i]->intersect(ray, t0, t1))
            {
				if (t0 < 0) t0 = t1;
                if ((t0 < t_close) && (t0 > 0.f))
                {
                    isInter = true;
                    t_close = t0;
                    i_close = i;
                }
			}
	}
	return isInter;
}