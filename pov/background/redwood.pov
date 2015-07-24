#include "colors.inc"

#declare XMAX = 1;
#declare XMIN = -1;

#declare YMAX = 5;
#declare YMIN = -1;


camera{
    orthographic
    location <(XMAX + XMIN)/2,(YMAX + YMIN)/2,-5>
    look_at  <(XMAX + XMIN)/2,(YMAX + YMIN)/2,0>
    up y*(YMAX - YMIN)
    right x*(XMAX - XMIN)
}

background{ LightBlue }

light_source{ <-1,5,-3> color rgb <1,1,1>*0.8 }
light_source{ <1,-1,-15> color rgb <1,1,1>*0.6 }

cone{ 0,0.9,y*4,0.5 pigment{ Brown }}