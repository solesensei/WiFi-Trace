#include "tracer.h"

using namespace glm;
using std::string;

SRay CTracer::MakeRay(float x, float y, float width, float height)
{
    return SRay::build_ray(x, y, width, height, camera);
}

std::tuple<uint,uint,uint> CTracer::vec3_to_color(v3 vector)
{
    uint x = uint(vector.x);
    uint y = uint(vector.y);
    uint z = uint(vector.z);
    return std::make_tuple(x, y, z);
}

v3 CTracer::MarchRay(const SRay& ray, v3 color, float t_closest){

    float depth = 0.0f;
    float step = t_closest/100000; // 1cm
    int index;
    int previous_index;
    bool first = true;
    for(int i=0; i < 100000; i++){
        v3 point = ray.orig + v3(ray.dir.x*depth, ray.dir.y*depth, ray.dir.z*depth);
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
            v3 wificolor = v3(0.f, 1.f, 0.f)*grid.voxels[index].value;
            wificolor = v3(0.f, glm::min(255.f,wificolor.y), 0.f);
            float alpha =0.04;
            color = color*(1-alpha) + wificolor*alpha;

        }
        else{
            // out of grid
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
    v3 res_color = v3(0.f, 0.f, 0.f);
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

    v3 pHit = ray.orig + ray.dir*t_closest; // hit point 
    v3 nHit; // normal at hit point
    pScene->figures[i_closest]->normalize(pHit, nHit);
    nHit = glm::dot(ray.dir, nHit) > 0 ? -nHit : nHit; // reverse normal if its inside 
    pHit = pHit + nHit*1e-4f;
   
    SRay shadow_ray;
    for(uint k = 0; k < pScene->lights.size(); ++k){
            // processing shadow parts
        v3 lightdir = pScene->lights[k].pos - pHit;
        lightdir = glm::normalize(lightdir);
        shadow_ray = SRay (pHit, lightdir); 
        bool isShadow = false;
        for(uint i = 0; i < pScene->figures.size(); ++i)
            if ( pScene->figures[i]->intersect(shadow_ray, t0, t1)){
                    isShadow = true; 
                    break;
            }
        if( !isShadow ){
            float dif = glm::dot(nHit, lightdir); 
            res_color += MarchRay(shadow_ray, pScene->figures[i_closest]->color * std::max(0.f, dif)*pScene->lights[k].intens, t_closest);
            v3 phong_color = SPhong::phong_calc(pScene->figures[i_closest]->color, // diffuse reflection constant
                                          v3(0.05f, 0.05f, 0.05f), // specular reflection constant
                                          5.f,  // shininess constant for this material
                                          nHit, 
                                          pHit,
                                          camera,
                                          pScene->lights[k]);
            res_color += phong_color;
        }
    }
    return vec3_to_color(res_color);
}

void CTracer::RenderImage(int width, int height, const char* name)
{
    Image res_image(height, width);
    for(uint i = 0; i < res_image.n_rows; ++i)
		for(uint j = 0; j < res_image.n_cols; ++j){	
			SRay ray = MakeRay(j, i, width, height);
			res_image(i, j) = TraceRay(ray);
		}
    pScene->SaveImageToFile(res_image, name);
}