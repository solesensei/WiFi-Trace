#include "tracer.h"

using namespace glm;
using std::string;

SRay CTracer::MakeRay(float x, float y)
{
    return SRay::build_ray(x, y, camera);
}

std::tuple<uint,uint,uint> CTracer::vec3_to_color(vec3 vector)
{
    uint x = uint(vector.x);
    uint y = uint(vector.y);
    uint z = uint(vector.z);
    return std::make_tuple(x, y, z);
}

vec3 CTracer::MarchRay(const SRay& ray, vec3 color, float t_closest){

    float depth = 0.0f;
    float step = 0.005;
    int index;
    int previous_index;
    bool first = true;
    float alpha = 0.05f;
    vec3 routerColor = vec3(0.0f,0.0f,0.0f);;
    for(int i = 0; i < MARCH_STEPS; ++i){
        vec3 point = ray.orig + vec3(ray.dir.x*depth, ray.dir.y*depth, ray.dir.z*depth);
        depth+=step;
        if(first){
            index = grid.find(point);
            first = false;
        }
        else{
            previous_index = index;
            index = grid.find(point);
            if(index==previous_index){
                continue;
            }
        }
        if(index>0){
            float value = grid.voxels[index].value;
            routerColor += vec3(value/7.f, value/30.f, value/1000.f);
            routerColor = glm::min(vec3(255.f,200.f,100.f), routerColor);

            color =  color * (1 - alpha) + routerColor*alpha;
        }
        else{ // out of grid
            return color;
        }
    }

    
    return color;
}

std::tuple<uint,uint,uint> CTracer::TraceRay(const SRay& ray)
{
    float t0, t1, t_closest = std::numeric_limits<float>::max();
    int i_closest; // index of the closest intersect object
    bool isInter = false;
    vec3 res_color = vec3(0.f, 0.f, 0.f);
    for(size_t i = 0; i < pScene->figures.size(); ++i){
            //check intersection
        if(pScene->figures[i]->intersect(ray, t0, t1))
        {    
            if (t0 < 0) t0 = t1;
            if (t0 < t_closest) 
            {
                isInter = true;
                t_closest = t0;
                i_closest = i;
            }

        }
    }
    if(!isInter) return vec3_to_color(res_color); // no intersection, returns black

    vec3 pHit = ray.orig + ray.dir*t_closest; // hit point 
    vec3 nHit = pScene->figures[i_closest]->normalize(pHit); // normal at hit point
    nHit = glm::dot(ray.dir, nHit) > 0 ? -nHit : nHit; // reverse normal if its inside 
    pHit = pHit + nHit*1e-2f;
   
    SRay shadow_ray;
    for(uint k = 0; k < pScene->lights.size(); ++k){
            // processing shadow parts
        vec3 lightdir = pScene->lights[k].pos - pHit;
        lightdir = glm::normalize(lightdir);
        shadow_ray = SRay (pHit, lightdir); 
        bool isShadow = false;
        for(uint i = 0; i < pScene->figures.size(); ++i)
            if ( pScene->figures[i]->intersect(shadow_ray, t0, t1))
            {
                if(t0 > 0)
                {
                    isShadow = true; 
                    break;
                }
            }
        if( !isShadow ){
            float dif = glm::dot(lightdir, nHit);
            if(dif < 0.f) 
                continue;
            SRay eye(pHit, ray.orig - pHit); 
            vec3 phong_color = SPhong::phong_calc(vec3(0.1f, 0.1f, 0.1f), // ambient reflection constant
                                                  pScene->figures[i_closest]->color*0.7f, // diffuse reflection constant
                                                  vec3(0.1f, 0.1f, 0.1f), // specular reflection constant
                                                  100.f,  // shininess constant for this material
                                                  nHit, 
                                                  pHit,
                                                  camera,
                                                  pScene->lights[k]);
            phong_color = glm::min(vec3(255.f, 255.f, 255.f), phong_color);
            res_color += MarchRay(eye, phong_color, t_closest);
        }
        res_color = glm::min(vec3(255.f, 255.f, 255.f), res_color);
    }
    return vec3_to_color(res_color);
}

void CTracer::RenderImage(int width, int height, const char* name)
{
    Image res_image(height, width);
    camera.width = float(width);
    camera.height = float(height);
    uint i,j;
    #pragma omp parallel for private(i,j)
    for(i = 0; i < res_image.n_rows; ++i)
		for(j = 0; j < res_image.n_cols; ++j){	
			SRay ray = MakeRay(j, i);
			res_image(i, j) = TraceRay(ray);
		}
    
    std::string path = string("../../img/") + name;
    pScene->save_image(res_image, path.c_str());
}
