#include <iostream>
#include <vector>
#include "geometry.h"
#include "tracer.h"
#include "types.h"

#define MODEL_PATH "../../models/room.obj"

int main(int argc, char** argv)
{
    /* Camera and Light creation */
  SCamera camera(v3(0.f, 0.f, 15.f));
  SLight light(v3(0.f, 20.f, -10.f), 3000.f);
    
    /* Model setting up */
  CModel model(MODEL_PATH);
  model.SetUpTriangles();
    
    /* Scene creation */
  CScene scene(MODEL_PATH);
  scene.cameras.push_back(camera);
  scene.lights.push_back(light);
  for(uint i = 0; i < model.triangles.size(); ++i)
		scene.figures.push_back(&(model.triangles[i]));
    
    /* Ray Tracer render setting up */
  CTracer tracer(camera, &scene);

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

  tracer.RenderImage(xRes, yRes, "result");
}