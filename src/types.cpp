#include "types.h"
using std::max;
using std::min;
using std::cerr;
using std::endl;

SCamera::SCamera(v3 Pos, v3 Up, v3 Right, float FOV):
    pos (Pos),
    up (Up),
    right (Right),
    front ( glm::normalize(glm::cross(up,right)) ), //vectors multiply, cross product
    fov (FOV)
{}

SRay SRay::build_ray(float x, float y, float width, float height, const SCamera& camera)
{
	float ratio = width / height;
	v3 ray_dir = camera.front + 
                 camera.right * ((x+0.5f) / width - 0.5f) * ratio - 
                 camera.up * ((y+0.5f) / height - 0.5f);

    return SRay( camera.pos, glm::normalize(ray_dir) );
}


SVoxel::SVoxel():
	pos (v3(0.f, 0.f, 0.f)),
	edge (0.5f),
	color (v3(1.f, 1.f, 1.f)),
	value (0.f)
{}


SVoxel::SVoxel(v3 Position, float Edge, v3 Color, float Value):
	pos (Position),
	edge (Edge),
	color (Color),
	value (Value)
{}

float SVoxel::toSDF(v3 point)
{
 	float x = glm::abs(point - pos).x - edge/2.f;
 	float y = glm::abs(point - pos).y - edge/2.f;
 	float z = glm::abs(point - pos).z - edge/2.f;
    return max(x, max(y,z));
}

float SVoxel::distance(SRay& ray, float start, float end)
{
	float depth = start;
    double eps = 1e-7;
	for(int i=0; i < STEPS; i++){
		v3 point = ray.orig + v3(ray.dir.x*depth, \
                                 ray.dir.y*depth, \
                                 ray.dir.z*depth);
			
		float dist = toSDF(point);
		if(dist < eps)
			return depth;

		depth += dist;
		if(depth >= end)
			return end;
	}
	return end;
}

SVoxelGrid::SVoxelGrid(v3 Topleft, v3 BotRight)
{
	topleft = Topleft;
	botright = BotRight;
	v3 subtract = glm::abs(BotRight - Topleft);
	float x = subtract.x;
	float y = subtract.y;
	float z = subtract.z;

	float min_axis = min(x, min(y,z));

	voxel_edge = min_axis/100;

		//calculate number of voxels per axis
	num_x = x/voxel_edge;
	num_y = y/voxel_edge;
	num_z = z/voxel_edge;
}


void SVoxelGrid::initialize(){
	for(uint i = 0; i < num_z; i++)
		for(uint j = 0; j < num_y; j++)
			for(uint k = 0; k < num_x; k++){
				v3 offset = topleft + v3(voxel_edge*0.5,voxel_edge*0.5,voxel_edge*0.5);
				voxels.push_back(
					SVoxel(
						offset+v3(voxel_edge*k,-voxel_edge*j,-voxel_edge*i),
						voxel_edge,
						v3(1.f, 0.f, 0.f) 
						)
					);
			}
}

int SVoxelGrid::find(v3 point){
	if(point.x > botright.x || point.y < botright.y || point.z < botright.z || 
 	   point.x < topleft.x  || point.y > topleft.y  || point.z > topleft.z)
			return -1;
	
	v3 subtract = point - topleft;
	subtract = glm::abs(subtract);

	size_t x = subtract.x/voxel_edge;
	size_t y = subtract.y/voxel_edge;
	size_t z = subtract.z/voxel_edge;

	return x + y*num_x + z*num_x*num_y;  
}

void SVoxelGrid::print()
{
	for(uint i=0; i < num_x*num_y*num_z; i++)
		if(voxels[i].value > 0)
			cerr << voxels[i].value << endl;
}