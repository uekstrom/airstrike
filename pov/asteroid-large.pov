#declare XMAX=1.1;
#declare XMIN=-XMAX;


#declare YMAX=1.1;
#declare YMIN=-YMAX;

#include "scene.inc"
#include "colors.inc"
#include "fun.inc"
#include "finish.inc"

blob{

sphere{<0,0,0>,1,1}
sphere{<0.1,0,0>,1,0.3}
sphere{<0.3,0.1,0>,0.5,1}
sphere{<0.3,0.1,-0.3>,0.4,1}
sphere{<-0.2,0.2,-0.1>,0.3,1}

threshold 0.65

	scale <1.6,1.3,1.6>
	translate <-0.1,-0.1,0>
	texture{
		pigment{ Gray40 }
//		normal{ bumps 0.1 scale 0.1 }
		normal{ wrinkles 0.9 scale 0.16 }
		finish{ specular 0.1 roughness 0.15 ambient 0.5 }
	}
	rotate y*clock*360
	rotate x*-10
}