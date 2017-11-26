#pragma once

#include <string>
#include "glm/glm.hpp"
#include "types.h"
#include "scene.h"


class CTracer
{
  std::tuple<uint,uint,uint> vec3_to_color(v3 vector);
public:
  SRay MakeRay(uv2 pixelPos);  // Create ray for specified pixel
  std::tuple<uint,uint,uint> TraceRay(const SRay& ray);
  void RenderImage(int xRes, int yRes, const char* name_of_image);
  
public:
  SCamera camera;
  CScene* pScene;

  CTracer(const SCamera& cam, CScene* scene): camera(cam), pScene(scene){}
};