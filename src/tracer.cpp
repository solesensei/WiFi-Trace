#include "tracer.h"

using namespace glm;

SRay CTracer::MakeRay(glm::uvec2 pixelPos)
{
  return SRay();
}

glm::vec3 CTracer::TraceRay(SRay ray)
{
  vec3 color(0, 0, 1);
  return color;
}

void CTracer::RenderImage(int xRes, int yRes)
{
  // Rendering
  m_camera.resolution = uvec2(xRes, yRes);
  m_camera.v_pixels.resize(xRes * yRes);

  for(int i = 0; i < yRes; i++)
    for(int j = 0; j < xRes; j++)
    {
      SRay ray = MakeRay(uvec2(j, i));
      m_camera.v_pixels[i * xRes + j] = TraceRay(ray);
    }
}

void CTracer::SaveImageToFile(std::string fileName)
{

}

Image CTracer::LoadImageFromFile(std::string fileName)
{
  Image i;
  return i;
}