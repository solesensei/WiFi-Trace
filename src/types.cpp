#include "types.h"
using std::max;
using std::min;
using std::cerr;
using std::endl;

SCamera::SCamera(vec3 Pos, vec3 Up, vec3 Right, float FOV):
    pos (Pos),
    up (Up),
    right (Right),
    front ( glm::normalize(glm::cross(up, right)) ), // vectors multiply, cross product
    fov (FOV)
{}

void SRay::set_strength(float s)
{
	strength = s;
}

SRay SRay::build_ray(float x, float y, const SCamera& cam)
{
	float ratio = cam.width / cam.height;

	vec3 ray_dir = cam.front + 
                   cam.right * ((x+0.5f) / cam.width - 0.5f) * ratio - 
                   cam.up * ((y+0.5f) / cam.height - 0.5f);

    return SRay( cam.pos, glm::normalize(ray_dir) );
}


SVoxel::SVoxel():
	pos (vec3(0.f, 0.f, 0.f)),
	side (0.5f),
	color (vec3(1.f, 1.f, 1.f)),
	value (0.f)
{}


SVoxel::SVoxel(vec3 Position, float Side, vec3 Color, float Value):
	pos (Position),
	side (Side),
	color (Color),
	value (Value)
{}

float SVoxel::calc_SDF(vec3 point)
{
	vec3 pp = glm::abs(point - pos) - side/2.f;
    return max(pp.x, max(pp.y, pp.z));
}

float SVoxel::calc_depth(SRay& ray, float start, float end)
{
	float depth = start;
    double eps = 1e-7;
	for(int i=0; i < MARCH_STEPS; ++i){
		vec3 point = ray.orig + vec3(ray.dir.x*depth, \
                                 	 ray.dir.y*depth, \
                                 	 ray.dir.z*depth);
			
		float dist = calc_SDF(point);
		if(dist < eps)
			return depth;

		depth += dist;
		if(depth >= end)
			break;
	}
	return end;
}

SVoxelGrid::SVoxelGrid(vec3 Topleft, vec3 Botright)
{
	topleft = Topleft;
	botright = Botright;
	vec3 sub = glm::abs(botright - topleft);

	float min_axis = min(sub.x, min(sub.y, sub.z));

	voxel_side = min_axis/10;

		// calculate number of voxels per axis
	num_x = sub.x / voxel_side;
	num_y = sub.y / voxel_side;
	num_z = sub.z / voxel_side;
}


void SVoxelGrid::initialize()
{
	for(uint k = 0; k < num_z; ++k)
		for(uint j = 0; j < num_y; ++j)
			for(uint i = 0; i < num_x; ++i){
				vec3 offset = topleft + vec3(voxel_side*0.5, voxel_side*0.5, voxel_side*0.5);
				voxels.push_back(SVoxel(offset + vec3(voxel_side*i, -voxel_side*j, -voxel_side*k),
						   		 		voxel_side,
						   		 		vec3(1.f, 0.f, 0.f)));
			}
}

int SVoxelGrid::find(vec3 point){
	if(point.x > botright.x || point.y < botright.y || point.z < botright.z || 
 	   point.x < topleft.x  || point.y > topleft.y  || point.z > topleft.z)
			return -1;
	
	vec3 sub = glm::abs(point - topleft);

	size_t x = sub.x/voxel_side;
	size_t y = sub.y/voxel_side;
	size_t z = sub.z/voxel_side;

	return x + y*num_x + z*num_x*num_y;  
}

void SVoxelGrid::print()
{
	for(uint i=0; i < num_x*num_y*num_z; ++i)
		if(voxels[i].value > 0)
			cerr << voxels[i].value << endl;
}