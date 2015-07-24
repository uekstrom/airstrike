

#declare XMAX=1.1;
#declare XMIN=-XMAX;


#declare YMAX=1.1;
#declare YMIN=-YMAX;

#include "scene.inc"
#include "colors.inc"

union{

cylinder{<0,-1,0>,<0,0.-0.8,0>,0.2 pigment{color Red}}
cylinder{<0,-0.8,0>,<0,0.2,0>,0.2 pigment{color White}}
cone{<0,1,0>,0,<0,0.2,0>,0.2 pigment{color Red}}


difference{
	box{<-1,1,-0.05>,<1,-1,0.05> rotate z*45}
	plane{<0,1,0>,0}
	pigment{color White}
	scale 0.5
	scale <1,2,1>
	translate y*-1}

difference{
	box{<-1,1,-0.05>,<1,-1,0.05> rotate z*45}
	plane{<0,1,0>,0}
	pigment{color White}
	scale 0.5
	scale <1,2,1>
	translate y*-1
	rotate y*90}

//	rotate y*sin(clock*4*pi)*5
//	rotate y*clock*360*4
	rotate -z*90
	rotate -z*clock*360
	scale 0.9
}