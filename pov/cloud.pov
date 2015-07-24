

#declare XMIN=-1.1;
#declare XMAX=1.1;

#declare YMIN=-0.55;
#declare YMAX=0.55;

#declare PREVIEW = 1;

#include "scene.inc"

#include "colors.inc"
#include "textures.inc"
#include "fun.inc"

#declare num = 300;

#declare I = seed(13);

union{
#while (num > 0)
sphere{<2.4*(fun6(rand(I))-0.5),
	pow(rand(I),4)*0.5-0.5,
	fun6(rand(I))-0.5>*0.5
     	, pow(rand(I),3)*0.25 }
#declare num = num - 1;
#end
texture{
pigment{White}
normal{wrinkles 0.3 scale 0.1}
finish{ ambient 0.3 }
}
scale 1.3
translate y*0.2
}