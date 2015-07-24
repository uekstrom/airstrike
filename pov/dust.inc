
#declare XMAX=1.1;
#declare XMIN=-XMAX;
#declare YMAX=1.1;
#declare YMIN=-YMAX;

#declare PREVIEW = 1;
#include "scene.inc"
#include "colors.inc"

#declare I = seed(2455);

union {
	#while (num > 0)
	sphere {
		#declare dist = 0.5 + 0.5*rand(I);
		0, 0.12 * (1 - sqrt(size_increment * dist * 1.5))
		translate dist * distance_multiplier * x
		rotate <rand(I)*360, rand(I)*360, rand(I)*360>
	}
	#declare num = num - 1;
	#end
	texture {
		pigment { rgb <0.5, 0.5, 0.5> }
		normal {
			wrinkles 0.3
			scale 0.1
		}
		finish { ambient 0.5 }
	}
}
