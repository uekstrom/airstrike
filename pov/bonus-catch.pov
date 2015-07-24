/*
 * An diminishing wreath of stars for Airstrike game bonuses
 *
 * (w) 2003 by Eero Tamminen
 */

#declare XMAX=1;
#declare XMIN=-XMAX;
#declare YMAX=1;
#declare YMIN=-YMAX;


#declare spike =
cone {
	<0, 1, 0>, 0
	<0, 0, 0>, 0.33
	translate 0.36 * y
}

// 72 degrees -> 5 spikes
#declare spikes = 360/5;

# declare star =
union {
	sphere {
		<0, 0, 0>, 0.5
	}
	#declare turn = 0;
	#while (turn < 360)
	object {
		spike
		rotate z * turn
	}
	#declare turn = turn + spikes;
	#end
}

#declare stars = 360/8;

union {
	#declare turn = 0;
	#while (turn < 360)
	object {
		star
		scale <0.3, 0.3, 0.3> * cos(clock*pi/2)
		translate (1 - 0.9*clock) * y
		rotate z * turn

		// animation
		rotate stars * clock * z
	}
	#declare turn = turn + stars;
	#end

	// scale and center around image
	scale <(XMAX-XMIN)/2*0.7, (YMAX-YMIN)/2*0.7, 1>
	translate <(XMAX+XMIN)/2, (YMAX+YMIN)/2, 0>

	// goes transparent with time
	pigment { rgbf <2, 2, 1, 0.5 * clock> }
}

camera {
	orthographic
	location <(XMAX + XMIN)/2,(YMAX + YMIN)/2,-5>
	look_at  <(XMAX + XMIN)/2,(YMAX + YMIN)/2,0>
	up y*(YMAX - YMIN)
	right x*(XMAX - XMIN)
	normal {
		// wave the view plane
		waves (0.01 + 0.02 * clock)
		phase pi * clock
		frequency 10
	}
}

light_source {
	<-1, 5, -3>*4
	color rgb<1,0.9,0.8>
	area_light
	20*x,20*y,5,5
	adaptive 4
	circular orient
}

light_source {
	<0, 0, -5>
	color rgb <1,1,1>*0.5 
	area_light
	10*x,10*y,5,5
	adaptive 4
	circular orient
}
