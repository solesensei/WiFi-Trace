#include <iostream>
#include <vector>
#include "geometry.h"
#include "tracer.h"
#include "types.h"
#include "argvparser.h"

using CommandLineProcessing::ArgvParser;
using std::cerr;
using std::cout;
using std::endl;

#define MODEL_PATH "../../models/new.obj"

int main(int argc, char** argv)
{
      /* Arguments parsing */
    ArgvParser cmd;
    cmd.setIntroductoryDescription("WiFi router signal coverage simulation");
    cmd.setHelpOption("h", "help", "print this help message");
    cmd.defineOption("config_file", "File with set up configuration", ArgvParser::OptionRequiresValue);
    int result = cmd.parse(argc, argv);
    cmd.defineOptionAlternative("config_file", "c");

        // Check for errors or help option
    if (result){
        cerr << cmd.parseErrorDescription(result) << endl;
        return result;
    }
    int xRes = 1024;  // Default resolution
    int yRes = 768;
    
    if(argc == 3) // There is input file in parameters
    {
      FILE* file = fopen(argv[2], "r");
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
    
      /* Camera and Light creation */
    // SCamera camera(v3(-2.f,30.f,-2.f),v3(0.f,0.f,-1.f),v3(1.f,0.f,0.f));
    SCamera front(v3(-9.f,0.f,0.f), v3(0.f,1.f,0.f), glm::normalize(v3(0.f,0.f,1.f)));
    SCamera top(v3(0.f,30.f,0.f), v3(0.f,0.f,-1.f), v3(1.f,0.f,0.f));
    SLight light(v3(0.f, 20.f, -10.f), 3000.f);
    SLight light2(v3(-6.f, 0.f, 1.f), 3000.f);
      /* Model setting up */
    CModel model(MODEL_PATH);

    // model.SetUp();
    SVoxelGrid grid(model.topleft, model.botright);
    grid.initialize();      
		SRouter router(v3(-6.f,0.f,0.f),0.2f,v3(1.f,0.f,0.f),20000.f);
    // router.fill_grid(grid,400000);

      /* Scene creation */
    CScene scene(model);
    // scene.cameras.push_back(camera);
    // scene.cameras.push_back(front);
    // scene.cameras.push_back(top);
    for(uint i = 0; i < model.triangles.size(); ++i)
        scene.figures.push_back(&(model.triangles[i]));


    router.fill_grid(scene.figures, grid, 20000);
    scene.figures.push_back(&router);
    scene.lights.push_back(light);
      /* Ray Tracer render setting up */
    // CTracer tracer(camera, &scene, grid);
    CTracer tracer_front(front, &scene, grid);
    CTracer tracer_top(top, &scene, grid);

    // tracer.RenderImage(xRes, yRes, "result.bmp");
    // tracer_front.RenderImage(xRes, yRes, "front.bmp");
    tracer_top.RenderImage(xRes, yRes, "top.bmp");
}