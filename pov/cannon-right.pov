
#declare XMAX=8;
#declare XMIN=-XMAX;

#declare YMIN=-3.2;
#declare YMAX=8.8;

//#declare PREVIEW = 1;
#include "scene.inc"

// set animation clock / cannon angle
#declare clk = clock;
#declare heading = 50;

#include "cannon.inc"
object {
	cannon
}
