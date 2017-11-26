#include <iostream>
#include <cmath>
#include <fstream>
#include <cstdlib>
#include "geometry.h"

using namespace std;

#define INF 1000000

Ray build_ray(double x, double y, double w, double h)
{
    double fov = 3.141592654/(2.0);
    vec3 ray_dir;
    ray_dir.x_ = x+0.5 - (w/2.0);
    ray_dir.y_ = y+0.5 - (h/2.0);
    ray_dir.z_ = -(w)/tan(fov/2.0);
    return Ray( vec3(0) , ray_dir.normalize() );
}

bool refract(vec3& ray_dir, vec3 nHit, double ior) 
{
    double eta = 1.0/ior; // eta = in_IOR/out_IOR
    double cosi = -nHit.dot(ray_dir);
    if(cosi < 0)
    {
        cosi *= -1;
        nHit = -nHit;
        eta = 1.0/eta;
    }
    double k = 1.0 - eta*eta*(1.0-cosi*cosi);
    if(k >= 0)
        ray_dir = ( ray_dir * eta +  nHit * (cosi*eta - sqrt(k)) ).normalize();
    return (k > 0);
}

#define MAX_DEPTH 3
#define FIGNUM 8
#define LIGHTNUM 3

Color ray_trace (Ray& ray, Figure** figures, const int depth)
{
    double t0,t1,tnear = INF;
    bool isInter = false; //intersection flag
    bool isShadow = false;
    Ray sRay; //shadow ray
    Light *lights[ LIGHTNUM ]; //array of lights
    Light L1 = Light( vec3 ( 250, -500, -30), 0.4);
    Light L2 = Light( vec3 (-300, -360, -40), 0.6);
    Light L3 = Light( vec3 (30, -200, -5), 0.2);
    lights[0] = &L1;
    lights[1] = &L2;
    lights[2] = &L3;
    Color resultCol;
    int nearInd = -1;
    for (int i = 0; i < FIGNUM; ++i) 
    {
        t0 = t1 = INF;
        if ( figures[i]->intersect(ray, t0 , t1) )
        {
            if (t0 < 0) t0 = t1;
            if (t0 < tnear) 
            {
                isInter = true;
                tnear = t0;
                nearInd = i;
            }
        }
    }
    
    if ( !isInter ) return resultCol; //no inserection

        double eps = 1e-4;
        vec3 pHit = ray.orig + ray.dir * tnear; //iserect point 
        vec3 nHit; //normal at pHit
        figures[nearInd]->getNorm (pHit,nHit);
        if ( ray.dir.dot(nHit) > 0)
            nHit = -nHit;
        //reflection or refraction
        if ( (figures[nearInd]->reflect > 0 || figures[nearInd]->transparent > 0) && depth < MAX_DEPTH)
        {
            Color reflCol, refrCol;
        //fresnel effect
            double facingratio = -ray.dir.dot(nHit);
            double fresnel = mix(pow(1 - facingratio, 3), 1, 0.1);
        //reflection
            vec3 refldir = ray.dir - nHit * 2 * ray.dir.dot(nHit);
            refldir.normalize();
            Ray reflRay = Ray(pHit + nHit * eps, refldir);
            reflCol = ray_trace(reflRay, figures, depth+1) * figures[nearInd]->reflect; 
        //refraction   
            if (figures[nearInd]->transparent)
                if ( refract(ray.dir,nHit,2) )
                    refrCol = ray_trace(ray, figures, depth + 1) * figures[nearInd]->transparent;
            resultCol = reflCol*fresnel + refrCol*(1 - fresnel);
        }
        //shadow rays
        for(int k = 0; k < LIGHTNUM; ++k)
        {
            vec3 lightdir = lights[k]->PT - pHit;
            lightdir.normalize();
            sRay = Ray (pHit + nHit*eps, lightdir); 
            isShadow = false;
            for(int i = 0; i < FIGNUM; ++i)
                if ( figures[i]->intersect(sRay, t0, t1)){
                     isShadow = true; 
                     break;
                }
            if( !isShadow ){
                double dif = nHit.dot( (lights[k]->PT - pHit).normalize() ); 
                resultCol = resultCol + figures[nearInd]->color * max(0, dif)*lights[k]->intens;
            }
        }

    return resultCol;

}



int main(int argc, char* argv[])
{
    int width,height;
	if (argc > 1)
    {
        if ( *argv[1] == 'f')
            width = 1000, height = 1000;
        else if ( *argv[1] == 'g')
            width = 2000, height = 2000;
        else if ( *argv[1] == 'b')
            width = 3000, height = 3000;
        else width = 2800, height = 2800;
    }
    else width = 2800, height = 2800;

    Color pixcolor;
    Figure *figures[ FIGNUM ]; //array of objects
    Sphere sp1 = Sphere ( vec3(-150, -45, -400), 45, Color(0,120,150), 0.9, 0 ) ;
    Sphere sp2 = Sphere ( vec3(35, -10, -160),  20,  Color(144,144,20), 0, 0.4 ) ;
    Sphere sp3 = Sphere ( vec3(26, -90, -300),  30,  Color(250,250,250), 0.3, 0 ) ;
    Cylindr cl1 = Cylindr ( vec3(-25,18,-145), 20, 12, Color(151,48,48), 0.5 , 0 ) ;
    Cylindr cl2 = Cylindr ( vec3(-20,33,-150), 10, 30, Color(135,58,203), 0.3 , 0 ) ;
    //Cone co1 = Cone ( vec3(-20,0,-150), 20, 12, Color(48,151,48), 0 , 0 ) ;
    Plane  pl1 = Plane  ( vec3(0, -1, 0), 38, Color(118,147,139), 0.42, 0) ;
    Plane  pl2 = Plane  ( vec3(0, 0, -1), -400, Color(30,30,27), 2, 0) ;
    Box bx1    = Box ( vec3(22,38,-140), vec3(48,10,-200),  Color(0,120,150), 0.1, 0 ) ;
    figures[0] = &sp1;
    figures[1] = &sp2;
    figures[2] = &sp3;
    figures[3] = &pl1;
    figures[4] = &cl1;
    figures[5] = &pl2;
    figures[6] = &bx1;
    figures[7] = &cl2;
	//creating image;
    ofstream image("picture.ppm");
	image << "P3" << endl;
	image << width << " " << height << endl;
	image << "255" << endl;

	for(int y = 0; y < height; y++)
		for(int x = 0 ; x < width; x++)
        {            
            Ray pRay = build_ray(x,y,width,height);
            pixcolor = ray_trace (pRay, figures, 0);
            image << (int) min(255, pixcolor.r) << " " << 
                     (int) min(255, pixcolor.g) << " " << 
                     (int) min(255, pixcolor.b) << endl;
        }
        
    image.close();


	return 0;
}
