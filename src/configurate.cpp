#include "configurate.h"

using std::cout;
using std::endl;
using std::stof;
using std::stoi;

ConfigParser::ConfigParser(const string path): file(path), isGood(true)
{}

vec3 ConfigParser::getVec()
{
	float x = stof(str);
	file >> str;
	float y = stof(str);
	file >> str;
	float z = stof(str);
	
    return vec3(x,y,z);
}

bool ConfigParser::setup()
{
    cout << "Config setup..." << endl;
    getResolution();
    getModel();
    getRouter();
    getCamera();
    getLight();
    if (isGood)
        cout << "Setup complete" << endl;
    else
        cout << "Setup terminated" << endl;
    return isGood;
}

void ConfigParser::launch()
{
    cout << "Launch render..." << endl;
    int xRes = v.width;  
    int yRes = v.height;
    const char* model_path = v.model_path.c_str();
    CModel model(model_path);
    SVoxelGrid grid(model.topleft, model.botright);
    grid.initialize();      
    SRouter router(v.r_position, v.r_rad, v.r_color, v.r_sig);

        /* Scene initialize */
    CScene scene(model);
    for(uint i = 0; i < model.triangles.size(); ++i)
        scene.figures.push_back(&(model.triangles[i]));


    router.fill_grid(scene.figures, grid, 20000);
    grid.filter();
    scene.figures.push_back(&router);
    scene.lights = v.lights;
    scene.cameras = v.cameras;
    
    CTracer tracer(&scene, grid);

    tracer.RenderImage(xRes, yRes);
    cout << "Render complete" << endl;    
}

bool ConfigParser::skip()
{ 
    if (!file.good())
        return false;

    file >> str;
    while(str[0] == '#'){
        getline(file, str);
        file >> str;
    }
    
    if (!file.good())
        return false;

    return true;
}

bool ConfigParser::next()
{
    file >> str;
    if(str[0] == '-'){
        getline(file, str);
        file >> str;
        return true;
    }
    getline(file, str);
    return false;
}

void ConfigParser::getResolution()
{
    if (!skip()){
        cout << "config error: no resolution found" << endl;
        isGood = false;
        return;
    }

    v.width = stoi(str);
    file >> str;
    v.height = stoi(str);
    
    cout << "Res: " << v.width << " x " << v.height << endl;
}

void ConfigParser::getModel()
{
    if (!skip()){
        cout << "config error: no model.obj path found" << endl;
        isGood = false;        
        return;
    }
    v.model_path = str.c_str();
    
    cout << "Model: " << v.model_path << endl;
}

void ConfigParser::getRouter()
{
    if (!skip()){
        cout << "config error: no router found" << endl;
        isGood = false;        
        return;
    }
    v.r_position = getVec();
    file >> str;
    v.r_rad = stof(str);
    file >> str;
    v.r_color = getVec();     
    file >> str;
    v.r_sig = stof(str);         

    cout << "Router placed" << endl;
}
void ConfigParser::getCamera()
{    
    if (!skip()){
        cout << "config error: no cameras found" << endl;
        isGood = false;        
        return;
    }
    int num = 1; // camera number
    do{
        cout << "Camera " << num++ << ": ";
        vec3 pos = getVec();
        file >> str;
        vec3 up = getVec();
        file >> str;
        vec3 right = getVec();
        file >> str;
        string name = str;
        SCamera camera(pos, up, right, name.c_str());
        v.cameras.push_back(camera);
        cout << name << " camera placed" << endl;
    }while(next()); 

}
void ConfigParser::getLight()
{
    if (!skip()){
        cout << "config error: no lights found" << endl;
        isGood = false;        
        return;
    }
    int num = 1; // light number
    do{
        cout << "Light " << num++ << ": ";
        vec3 pos = getVec();
        file >> str;
        float intens = stof(str);
        SLight light(pos, intens);
        v.lights.push_back(light);
        cout << "light placed" << endl;
    }while(next()); 
    
}