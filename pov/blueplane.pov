
#declare XMIN=-1.1;
#declare XMAX=1.1;
#declare YMIN=-1.1;
#declare YMAX=1.1;

//#declare PREVIEW = 1;
#include "scene.inc"

#declare broken = 0;
#declare plane_color = color rgb <0.2,0.4,1>;
#include "plane.inc"

object {
	Plane
//cw
	rotate y*180
	rotate -z*clock*360
}
