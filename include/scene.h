#pragma once
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "geometry.h"

class CModel;

class CScene
{
public:
	std::vector<Figure*> figures;
	std::vector<SLight> lights;
	std::vector<SCamera> cameras;

		// Load scene model from .obj file
	void load_model(const char* path);
		// Save result image to .bmp file	
	void save_image(const Image &im, const char *path);
		// Load image from file	
	Image load_image(const char *path);
		// Place wifi router at scene
	void place_router();

public:
	CModel* model;

	CScene(const char* path_to_model);
	CScene(CModel& loaded_model);
};

class CMesh
{
public:
	std::vector<vec3> vertices;
	std::vector<vec3> normals;

	CMesh(std::vector<vec3> vertices,std::vector<vec3> normals);
};

class CModel
{
	float min_x, min_y, min_z;
	float max_x, max_y, max_z;
public: 
		// Import model with assimp  	
 	void load(const char* filename);
	 	// Calculate assimp node meshes
 	void calc_node(aiNode *node, const aiScene *scene);
		// Set up model filling meshes and triangles
 	void initialize();
		// Convert assimp mesh to CMesh
	CMesh getMesh(aiMesh *mesh);
		// Check box overflows and correct max,min values
	void checkBound(glm::vec3 vertice);
		// Initialize topleft and botright with bounds
 	void setBound();
 
public:
	std::vector<STriangle> triangles;
	std::vector<CMesh> meshes;
	vec3 topleft;
	vec3 botright;
	vec3 position;

	CModel(const char* filename, const vec3& pos = vec3(0.f,0.f,0.f));
};