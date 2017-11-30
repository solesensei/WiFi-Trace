# Wifi Tracing
Visualization of WiFi coverage with ray tracing algorithm
---
## Build and Launch

        make
        cd build/bin
        ./visualize [-c] [config_file]
        
        ./visualize --help

### Launch script 

        bash test.sh # build and run with config_file
   
---
## Work algoritm

### Basic part

1. Read config_file and setup scene
2. Build and fill voxel grid with ray marching algorithm
3. Antialiasing grid with box filter
4. Visializate scene with camera back ray tracing
5. Colorize scene with ray marching grid
6. Save result to image.bmp

### Additional part

* OpenMP parallel
* Phong filter

## Usefull links
[Scratchapixel](https://www.scratchapixel.com/lessons/3d-basic-rendering/introduction-to-ray-tracing/implementing-the-raytracing-algorithm])

[Ray Tracing by Frolov](http://ray-tracing.ru/)

[Ray Marching algorithm](http://jamie-wong.com/2016/07/15/ray-marching-signed-distance-functions/#the-raymarching-algorithm)

## Used libraries 
[EasyBMP](http://easybmp.sourceforge.net/)

[glm](https://github.com/g-truc/glm)

[assimp](https://github.com/assimp/assimp)