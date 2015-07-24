#include "as-textures.inc"

#declare XMAX = 1.2;
#declare XMIN = -1.2;

#declare YMAX = 1.2;
#declare YMIN = -1.2;

#include "scene.inc"

box{ -1,<1,0,1> texture{ tex_browndirt }}
union{
cylinder{ <-1,0,-1>,<1,0,-1> 0.1 }
//sphere{ <-1,0,-1> 0.1}
//sphere{ <1,0,-1> 0.1}

texture{ tex_moss }
}