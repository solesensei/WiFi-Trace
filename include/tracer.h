#pragma once

#include <string>
#include "glm/glm.hpp"
#include "types.h"
// #include "scene.h"


class CTracer
{
public:
  SRay MakeRay(uv2 pixelPos);  // Create ray for specified pixel
  glm::vec3 TraceRay(SRay ray); // Trace ray, compute its color
  void RenderImage(int xRes, int yRes);
  static void SaveImageToFile(const Image &im, const char *path);
  static Image LoadImageFromFile(const char *path);

public:
  SCamera m_camera;
  // CScene* m_pScene;
};