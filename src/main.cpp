#include <iostream>
#include <vector>
#include "geometry.h"
#include "tracer.h"


int main(int argc, char** argv)
{
  CTracer tracer;
  Image pic = CTracer::LoadImageFromFile("./img/Lenna.bmp");
  // CScene scene;


  int xRes = 1024;  // Default resolution
  int yRes = 768;
  
  if(argc == 2) // There is input file in parameters
  {
    FILE* file = fopen(argv[1], "r");
    if(file)
    {
      int xResFromFile = 0;
      int yResFromFile = 0;
      if(fscanf(file, "%d %d", &xResFromFile, &yResFromFile) == 2)
      {
        xRes = xResFromFile;
        yRes = yResFromFile;
      }
      else
        printf("Invalid config format! Using default parameters.\r\n");

      fclose(file);
    }
    else
      printf("Invalid config path! Using default parameters.\r\n");
  }
  else
    printf("No config! Using default parameters.\r\n");

  // tracer.m_pScene = &scene;
  tracer.RenderImage(xRes, yRes);
  tracer.SaveImageToFile(pic, "result.bmp");
}