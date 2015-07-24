

#declare XMAX=1.1;
#declare XMIN=-XMAX;


#declare YMAX=1.1;
#declare YMIN=-YMAX;

#include "scene.inc"
#include "colors.inc"

box{
	<-0.3,0.7,-1>,<0.3,-0.7,1>
	texture{
	pigment{ Brown }
	}
	rotate -z*clock*360
}