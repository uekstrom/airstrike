

#declare XMAX=1.1;
#declare XMIN=-XMAX;


#declare YMAX=1.1;
#declare YMIN=-YMAX;

#include "scene.inc"
#include "colors.inc"
#include "finish.inc"

union{
union{
sphere{ 0,1 }
cylinder{ 0,y*1.2,0.3 }
pigment{ Gray10 } finish{ Dull } 
}
cylinder {
		0, y*1.4, 0.2
		pigment { color rgb <1,0.5*sin(clock/12)+0.5,0.2> }
		finish{ ambient 1 }	
	}
scale 0.8
rotate z*-30
translate y*-0.1 
}
