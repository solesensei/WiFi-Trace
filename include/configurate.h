#pragma once
#include <fstream>
#include "types.h"
#include "geometry.h"
#include "tracer.h"
#include "scene.h"
#include "ColorTerminal.h"

using std::ifstream;
using std::string;
using std::vector;

struct SValue
{
    int width;
    int height;
    string model_path;
    vec3 r_position;
    vec3 r_color;
    float r_rad;
    float r_sig;
    
    vector <SCamera> cameras;
    vector <SLight> lights;

    SValue(){}
};

class ConfigParser
{
    ifstream file; // config filename
    string str; // current read string
    SValue v; // all values in one structure
    bool isGood; // flag checks state of setup
public:
    
    ConfigParser(const string path);


    bool setup(); 
    void launch();

    vec3 getVec(); // read vec3 from file
    bool skip(); // skip comments and return string to process
    bool next(); // check on several objects in one section 
    void getResolution();
    void getModel();
    void getRouter();
    void getCamera();
    void getLight();
};