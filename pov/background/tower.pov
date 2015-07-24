#declare XMIN = -1.5;
#declare XMAX = 1.5;

#declare YMIN = -0.5;
#declare YMAX = 5.5;

#include "scene.inc"
#include "as-textures.inc"

/* Create a unit cylinder slice, with the
	cylinder along the y axis. Can only
	do slices < 180 degrees*/
#macro cylinder_slice(ang_start,ang_end)
difference{
	cylinder{0,y,1}
	plane{x,0 rotate y*ang_start}
	plane{-x,0 rotate y*ang_end }
}
#end

#declare brickseed = seed(1723);

#macro brick_layer(_mortar,_bricksize)
union{
#local a1 = 0;
#while (a1 < 360)
#local radd = rand(brickseed)*15;
cylinder_slice(a1, a1 + _bricksize + radd)
#local a1 = a1 + _bricksize + _mortar + radd;
#end
}
#end


union{
	cylinder{ 0,3*y,0.93 pigment{ Gray60 }
	texture{ tex_climbingmoss scale 1.2}
	}
#declare _y = 0;
#while (_y < 3)	
object{
	brick_layer(3,15)
	rotate _y*7*29*pi*y
	scale <1,0.25,1>
	translate y*_y
#declare _y = _y + 0.30;
}
#end
difference{
object{
	brick_layer(10,15)
	scale <1.1,0.55,1.1>
	translate y*3
	rotate y*-85
}
cylinder{0,5*y,0.7}
}
texture{ tex_rock }
	texture{ tex_climbingmoss}
} 