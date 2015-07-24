

#declare XMIN=-1.1;
#declare XMAX=1.1;

#declare YMIN=-1.1;
#declare YMAX=1.1;

#declare PREVIEW = 1;

#include "scene.inc"

#include "colors.inc"
#include "textures.inc"
#include "fun.inc"

#declare num = 60;

#declare I = seed(2451);

#declare time = mod(clock,1.0)*0.7 + 0.2;
#declare animparam = int(clock);

union{
#while (num > 0)
sphere{ <1.2,0,0>*time*fun1(rand(I)) 
	0.08*fun1(time) 
	rotate z*(rand(I) - 0.5)*30 
}
#declare num = num - 1;
#end
texture{
pigment{Gray60}
normal{wrinkles 0.3 scale 0.1}
finish{ ambient 0.5 }
}
rotate -z*animparam*360/32.0
}
