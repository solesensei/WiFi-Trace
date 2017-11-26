#include "scene.h"
#include "tracer.h"

using std::string;
using std::cerr;
using std::endl;

CScene::CScene(const char* path)
{
	LoadModel(path);
}

CScene::CScene(const CModel& loaded_model)
{
	*model = loaded_model;
}

void CScene::LoadModel(const char* path)
{
	*model = CModel(path);
}

void CScene::SaveImageToFile(const Image &im, const char *path)
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

Image CScene::LoadImageFromFile(const char *path)
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

void CScene::PlaceRouter()
{
	SRouter router(v3(0.f, 0.f, 0.f), 200.f, v3(1.f, 0.f, 0.f));
}

CMesh::CMesh(std::vector<v3> Vertices, std::vector<v3> Normals)
{
    vertices = Vertices;
	normals = Normals;
}

CModel::CModel(const char* filename)
{
    load(filename);
}

void CModel::load(const char* filename)
{
	Assimp::Importer import;
    const aiScene *scene = import.ReadFile(filename, aiProcess_Triangulate);	
	
    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) 
    {
        cerr << "Assimp load error: " << import.GetErrorString() << endl;
        return;
    }

    processNode(scene->mRootNode, scene);
}

void CModel::processNode(aiNode *node, const aiScene *scene)
{
    for(uint i = 0; i < node->mNumMeshes; ++i){
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]]; 
        meshes.push_back(processMesh(mesh, scene));			
    }
    // then do the same for each of its children
    for(uint i = 0; i < node->mNumChildren; ++i){
        processNode(node->mChildren[i], scene);
    }
}  

CMesh CModel::processMesh(aiMesh *mesh, const aiScene *scene)
{
    std::vector<v3> vertices;
    std::vector<v3> normals;

    for(uint i = 0; i < mesh->mNumVertices; ++i){
    	v3 vector;
    	vector.x = mesh->mVertices[i].x;
    	vector.y = mesh->mVertices[i].y;
    	vector.z = mesh->mVertices[i].z;
        vertices.push_back(vector);
        vector.x = mesh->mNormals[i].x;
        vector.y = mesh->mNormals[i].y;
        vector.z = mesh->mNormals[i].z;
        normals.push_back(vector);
    }

    return CMesh(vertices, normals);
}  

void CModel::SetUpTriangles()
{
	for(size_t i = 0; i < meshes.size(); ++i){
		for(size_t j =0; j < meshes[i].vertices.size(); j+=3){
			v3 a = meshes[i].vertices[j];
			v3 b = meshes[i].vertices[j+1];
			v3 c = meshes[i].vertices[j+2];
			STriangle temp(a,b,c,meshes[i].normals[j]);
			triangles.push_back(temp);
		}
	}
}