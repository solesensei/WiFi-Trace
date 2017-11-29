#pragma once

#include <string>
#include "glm/glm.hpp"
#include "types.h"
#include "scene.h"


class CTracer
{
        // Convert vec3 to tuple
    std::tuple<uint,uint,uint> vec3_to_color(vec3 vector);
public:
        // Render scene with ray tracing and save it to file
    void RenderImage(int xRes, int yRes, const char* name_of_image);
        // Create ray for specified scene pixel
    SRay MakeRay(float x, float y);
        // Check ray intersection point and calculate its color 
    std::tuple<uint,uint,uint> TraceRay(const SRay& ray);
        // TraceRay processing part, ray marching algorithm using voxel grid   
    vec3 MarchRay (const SRay& ray, vec3 color, float t_closest);

    // vec3 ColorRay ()
  
public:
    SCamera camera;
    CScene* pScene;
    SVoxelGrid grid;

  CTracer(const SCamera& cam, CScene* scene, const SVoxelGrid& g): camera(cam), pScene(scene), grid(g) {}
};
