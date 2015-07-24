#include "as-textures.inc"

#declare XMAX = 1.2;
#declare XMIN = -1.2;

#declare YMAX = 0.6;
#declare YMIN = -0.6;

#include "scene.inc"

blob{
threshold 0.65
sphere{ 0,1,1 scale <2,0.1,2> }
sphere{ -y*0.3,0.4,1 scale <0.6,1,0.6> }
sphere{ -y*0.2,1,0.9 scale <1,0.2,1>}
pigment{White}
}