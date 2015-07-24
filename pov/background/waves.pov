#include "colors.inc"

#declare XMAX = 1;
#declare XMIN = -1;

#declare YMAX = 1;
#declare YMIN = -1;


camera{
    orthographic
    location <(XMAX + XMIN)/2,(YMAX + YMIN)/2,-5>
    look_at  <(XMAX + XMIN)/2,(YMAX + YMIN)/2,0>
    up y*(YMAX - YMIN)
    right x*(XMAX - XMIN)
}

light_source{ <-1,5,-3> color rgb <1,1,1>*0.8 }
light_source{ <1,-1,-15> color rgb <1,1,1>*0.6 }


#declare nr_wave_cyls = 200;

blob{ threshold 0.8

	sphere{ <0,0,0> 0.7,0.8 scale <1,0.3,1> rotate z*60}
	sphere{ <0,0,0> 1.5,0.8 scale <1,0.3,1> translate -0.4*y}
	pigment{ White }

}

