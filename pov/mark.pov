
#declare XMIN=-1.1;
#declare XMAX=1.1;

#declare YMIN=-1.1;
#declare YMAX=1.1;

#include "scene.inc"
#include "colors.inc"

union{
	
	cone{<0,1,0>,0.2,<0,0,0>,0}
	cone{<0,-1,0>,0.2,<0,0,0>,0}
	cone{<-1,0,0>,0.2,<0,0,0>,0}
	cone{<1,0,0>,0.2,<0,0,0>,0}
	pigment{Green}
	rotate z*clock*90
}