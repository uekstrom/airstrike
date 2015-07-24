

#declare XMAX=1.1;
#declare XMIN=-XMAX;


#declare YMAX=1.1;
#declare YMIN=-YMAX;

#include "scene.inc"
#include "colors.inc"

union{

	sphere{ <0,0,0>,1 scale <0.3,1,0> 
		texture{
		pigment{ Gray20 }
		finish{Glossy}
		}
	}

difference{
	box{<-1,1,-0.05>,<1,-1,0.05> rotate z*45}
	plane{<0,1,0>,0}
	pigment{color Red}
	scale 0.5
	scale <1,3,1>
	translate y*-1}

difference{
	box{<-1,1,-0.05>,<1,-1,0.05> rotate z*45}
	plane{<0,1,0>,0}
	pigment{color Red}
	scale 0.5
	scale <1,3,1>
	translate y*-1
	rotate y*90}

	rotate -z*90
	rotate -z*clock*360
	scale 0.9
}