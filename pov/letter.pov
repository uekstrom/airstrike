
#declare XMIN=-1.1;
#declare XMAX=1.1;

#declare YMIN=-1.1;
#declare YMAX=1.1;

//#declare PREVIEW = 1;

#include "scene.inc"

#include "colors.inc"
#include "textures.inc"
#include "finish.inc"
#include "fun.inc"

box{ <-1,-0.5,-0.001> <1,0.5,0.001> pigment{ White } 
	scale 0.5
	//rotate x*per3(clock,30,10,5)
	rotate x*30
	translate -0.5*y
	rotate z*per2(clock,0,30)
	translate z*1}