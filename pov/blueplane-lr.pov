
#declare XMIN=-1.1;
#declare XMAX=1.1;
#declare YMIN=-1.1;
#declare YMAX=1.1;

//#declare PREVIEW = 1;
#include "scene.inc"

#declare broken = 0;
#declare plane_color = color rgb <0.3,0.5,1>;
#include "plane.inc"

object {
	Plane

// turn right->left

	rotate x*45*pow(sin(clock*pi),2)
	rotate -y*180*clock //*(clock*( clock/2 - clock*clock/3)*1/(1/2.0 - 1/3.0))
	//rotate -y*360*clock
}
