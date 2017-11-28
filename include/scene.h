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

  void LoadModel(const char* path);
  void SaveImageToFile(const Image &im, const char *path);
  Image LoadImageFromFile(const char *path);
  void PlaceRouter();
public:
  CModel* model;

  CScene(const char* path_to_model);
  CScene(CModel& loaded_model);
};

class CMesh
{
 public:
	std::vector<v3> vertices;
	std::vector<v3> normals;

	CMesh(std::vector<v3> vertices,std::vector<v3> normals);

};

class CModel
{
	float min_x, min_y, min_z;
	float max_x, max_y, max_z;
public: 	
 	void load(const char* filename);
 	void ProcessNode(aiNode *node, const aiScene *scene);
 	CMesh ProcessMesh(aiMesh *mesh, const aiScene *scene);
 	void SetUp();
	void CheckBoundingBox(glm::vec3 vertice);
 	void FindBoundingBox();
 
public:
	std::vector<STriangle> triangles;
	std::vector<CMesh> meshes;
	v3 topleft;
	v3 botright;

	CModel(const char* filename);
};