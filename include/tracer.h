#pragma once

#include <string>
#include "glm/glm.hpp"
#include "types.h"
#include "scene.h"


class CTracer
{
  std::tuple<uint,uint,uint> vec3_to_color(v3 vector);
public:
  SRay MakeRay(float x, float y, float width, float height);  // Create ray for specified pixel
  v3 MarchRay (const SRay& ray, v3 color, float t_closest);
  std::tuple<uint,uint,uint> TraceRay(const SRay& ray);
  void RenderImage(int xRes, int yRes, const char* name_of_image);
  
public:
  SCamera camera;
  CScene* pScene;
  SVoxelGrid grid;

  CTracer(const SCamera& cam, CScene* scene, const SVoxelGrid& g): camera(cam), pScene(scene), grid(g) {}
};
