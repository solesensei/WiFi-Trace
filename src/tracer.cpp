#include "tracer.h"

using namespace glm;
using std::string;
SRay CTracer::MakeRay(glm::uvec2 pixelPos)
{
  pixelPos = uv2();
  return SRay();
}

glm::vec3 CTracer::TraceRay(SRay ray)
{
  ray = SRay();
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

void CTracer::SaveImageToFile(const Image &im, const char *path)
{
    BMP out;
    out.SetSize(im.n_cols, im.n_rows);

    uint r, g, b;
    RGBApixel p;
    p.Alpha = 255;
    for (uint i = 0; i < im.n_rows; ++i) {
        for (uint j = 0; j < im.n_cols; ++j) {
            std::tie(r, g, b) = im(i, j);
            p.Red = r; p.Green = g; p.Blue = b;
            out.SetPixel(j, i, p);
        }
    }

    if (!out.WriteToFile(path))
        throw string("Error writing file ") + string(path);
}

Image CTracer::LoadImageFromFile(const char *path)
{
    BMP in;

    if (!in.ReadFromFile(path))
        throw string("Error reading file ") + string(path);

    Image res(in.TellHeight(), in.TellWidth());

    for (uint i = 0; i < res.n_rows; ++i) {
        for (uint j = 0; j < res.n_cols; ++j) {
            RGBApixel *p = in(j, i);
            res(i, j) = std::make_tuple(p->Red, p->Green, p->Blue);
        }
    }

    return res;
}