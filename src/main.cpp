#include <iostream>
#include <vector>
#include "geometry.h"
#include "tracer.h"
#include "types.h"
#include "argvparser.h"
#include "configurate.h"
#include "ColorTerminal.h"

using CommandLineProcessing::ArgvParser;
using std::cout;
using std::cerr;
using std::endl;

void default_run()
{
    cout << ycol << "Default setup..." << reset << endl;
    int xRes = 1920;  // Default resolution
    int yRes = 1080;
    cout << wbcol << "Res: " << reset << xRes << " x " << yRes << endl;
    const char* model_path = "../../models/room1.obj";
    cout << wbcol << "Model: " << reset << model_path << endl;
        /* Camera and Light creation */
    SCamera front(vec3(-13.f,3.f,5.f), glm::normalize(vec3(0.5f,1.f,0.f)), glm::normalize(vec3(0.4f,0.f,1.f)), "front");
    SCamera top(vec3(0.f,20.f,0.f), vec3(0.f, 0.f, -1.f), vec3(1.f,0.f,0.f), "top");
    SLight light(vec3(-6.f, 10.f, -10.f), 10.f);
    SLight light2(vec3(20.f, 20.f, 1.f), 1.f);
    cout << wbcol << "Camera 1: " << reset << "front camera placed" << endl;
    cout << wbcol << "Camera 2: " << reset << "top camera placed" << endl;
    cout << wbcol << "Light 1: "  << reset << "placed" << endl;
    cout << wbcol << "Light 2: "  << reset << "placed" << endl;
    cout << ycol << "Setup complete" << reset << endl;

    cout << ccol << "Launch render..." << reset << endl;
        /* Model setting up */
    CModel model(model_path);
    cout << "Model loaded" << endl;
    SVoxelGrid grid(model.topleft, model.botright);
    grid.initialize();      
    cout << "Voxel grid initialized" << endl;
    SRouter router(vec3(-6.f,0.f,0.f),0.2f,vec3(1.f,0.f,0.f),10000.f);

        /* Scene initialize */
    CScene scene(model);
    for(uint i = 0; i < model.triangles.size(); ++i)
        scene.figures.push_back(&(model.triangles[i]));
    cout << "Scene initializing..." << endl;

    router.fill_grid(scene.figures, grid, 20000);
    cout << "Router filled the grid" << endl;
    grid.filter();
    cout << "Voxel filter applied" << endl;
    scene.figures.push_back(&router);
    scene.lights.push_back(light);
    scene.lights.push_back(light2);
    scene.cameras.push_back(front);
    scene.cameras.push_back(top);
    
    CTracer tracer(&scene, grid);
    cout << "Ray Tracer initialized" << endl;
    cout << ccol << "Start rendering..." << reset << endl;
    tracer.RenderImage(xRes, yRes);
    cout << ccol << "Render complete" << reset << endl;
}

void config_run(string file)
{
    ConfigParser config(file);
    bool set = config.setup();
    if (set)
        config.launch();
}

int main(int argc, char** argv)
{

    cout << gcol << "\nVisualization of WiFi coverage with ray tracing algorithm" << " by Sole\n\n" << reset;

        /* Arguments parsing */
    ArgvParser cmd;
    cmd.setIntroductoryDescription("WiFi router signal coverage simulation");
    cmd.setHelpOption("h", "help", "print this help message");
    cmd.defineOption("config_file", "File with set up configuration", ArgvParser::OptionRequiresValue);
    cmd.defineOptionAlternative("config_file", "c");

    int result = cmd.parse(argc, argv);

        // Check for errors or help option
    if (result){
        cerr << cmd.parseErrorDescription(result) << endl;
        return result;
    }
    
    Timer t; // measure runtime
    t.start();

    bool setup = cmd.foundOption("config_file");
    if (setup){
        string file = cmd.optionValue("config_file");
        config_run(file);
    }
    else{
        cout << wbcol << "No config! Using default parameters." << reset <<endl;
        default_run();
    }    

    t.check("\nCPU runtime");
    
    return 0;
}