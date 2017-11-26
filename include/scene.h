#pragma once
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
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
  CScene(const CModel& loaded_model);
};

class CMesh
{
 public:
	std::vector<v3> vertices;
	std::vector<v3> normals;

	CMesh(std::vector<v3> vertices,std::vector<v3> normals);

	//SetUp();
};

class CModel
{
public: 	
 	void load(const char* filename);
 	void processNode(aiNode *node, const aiScene *scene);
 	CMesh processMesh(aiMesh *mesh, const aiScene *scene);
 	void SetUpTriangles();

public:
	std::vector<STriangle> triangles;
	std::vector<CMesh> meshes;

	CModel(const char* filename);
};