
#declare XMAX=1;
#declare XMIN=-XMAX;

#declare YMAX=1;
#declare YMIN=-YMAX;

#include "scene.inc"

box {
	<-1,-1,-1>,<1,1,1>
	pigment{
		color rgb <0.1, 0.1, 0.1>
	}
	finish{
		ambient 1
	}
	scale 0.7
}
