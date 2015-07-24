

#declare XMIN=-1.1;
#declare XMAX=1.1;

#declare YMIN=-1.1;
#declare YMAX=1.1;

#declare PREVIEW = 1;

#include "scene.inc"

#include "colors.inc"
#include "textures.inc"
#include "fun.inc"

#declare num = 100;

#declare I = seed(2451);

union{
#while (num > 0)
#declare age = fun1(rand(I));
sphere{ <0,2.0,0>*clock*age
	0.18*fun4(age)*fun1(clock)
	rotate z*(rand(I) - 0.5)*(90 + clock*clock*30) 
	pigment{ White*(1-clock) + (Orange + Red*rand(I)*0.2)*fun3(clock) + Gray50*fun5(clock)}
}
#declare num = num - 1;
#end
texture{
	normal{wrinkles 0.3 scale 0.1}
	finish{ ambient 1.0}
	}
translate <0,-1,0>
}

#declare num = 60;

union{
#while (num > 0)
#declare age = fun1(rand(I));
sphere{ <0,2.0,0>*fun0(clock - 0.2)*age
	0.1*fun4(age)*fun1(fun0(clock - 0.2))  
	rotate z*(rand(I) - 0.5)*(90 + clock*clock*30)
	pigment{ Gray50 }
}
#declare num = num - 1;
#end
texture{
	normal{wrinkles 0.3 scale 0.1}
	finish{ ambient 1.0*(1 - fun0(clock - 0.2)) }
	}
translate <0,-1,0>
}
