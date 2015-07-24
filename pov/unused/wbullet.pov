
#declare XMIN=-1.1;
#declare XMAX=1.1;

#declare YMIN=-1.1;
#declare YMAX=1.1;

#include "scene.inc"

cone{

	<-1,0,0>,0.1,<1,0,0>,0.0
	texture{
	pigment{color rgb <1,1,1>}
	finish{ambient 1}
	}
	rotate -z*clock*360
}