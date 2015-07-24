
#declare XMIN=-1.1;
#declare XMAX=1.1;

#declare YMIN=-1.1;
#declare YMAX=1.1;

#include "scene.inc"

//box{<-1,-1,2>*1.0,<1,1,1>*1.0 pigment{color rgb <0,1,0>}}

union{

union{

	difference{
		sphere{<0,0,0>,0.5 }
	box{<-1,0,-1>,<1,-1,1>}
	scale <1,0.8,1>
	texture{Polished_Chrome}
	}

	sphere{<0,0,0>,1 
		scale <1,0.2,1>
	texture{Soft_Silver}
        pigment{color rgb <1,1,1>}
	normal{dents 1.0 scale 0.05}
	}
}

union{
sphere{<0.7,0.1,0>,0.1 }
sphere{<-0.7,0.1,0>,0.1 }
sphere{<0,0.1,0.7>,0.1 }
sphere{<0,0.1,-0.7>,0.1 }
texture{Polished_Brass}
}

//rotate y*clock*360
//rotate x*10*sin(clock*2*pi)
  rotate -z*clock*360
}
