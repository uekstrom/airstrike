/*
 * A cute little dragon for Airstrike
 *
 * (w) 2003 by Eero Tamminen
 */

/* show only legs
#declare XMAX=4.6;
#declare XMIN=-XMAX;
#declare YMAX=1.5;
#declare YMIN=-5.4;
*/
#declare XMAX=9.2;
#declare XMIN=-XMAX;
#declare YMAX=8.4;
#declare YMIN=-5.4;
#include "scene.inc"

#include "colors.inc"


#declare toe =
union {
	sphere {
		0, 0.6
		scale <1, 0.5, 0.5>
	}
	// claw
	sphere {
		0, 0.2
		scale <1, 0.5, 0.5>
		translate 0.5 * x
		pigment { White }
	}
}

#declare legmove = 0.2 * sin(clock*2*pi);

#declare leg =
union {
	sphere {
		// hip -> knee
		0, 1.1
		scale <2, 1, 1>
		rotate (10 + 10 * sin(clock*2*pi)) * z
	}
	sphere {
		// knee -> angle
		0, 0.9
		scale <2, 1, 1>
		rotate (50 - 10 * sin(clock*2*pi)) * z
		translate <0.5 - legmove, -0.5 + legmove, 0>
	}
	union {
		object {
			// toe backward
			toe
			rotate 180 * y
			translate -0.3 * x
			rotate (25 + 15 * sin(clock*2*pi)) * z
		}
		sphere {
			// toe one
			toe
			rotate -30 * y
			rotate -(25 + 15 * sin(clock*2*pi)) * z
			translate 0.2 * z
			translate 0.3 * x
		}
		sphere {
			// toe two
			toe
			rotate 30 * y
			rotate -(25 + 15 * sin(clock*2*pi)) * z
			translate -0.2 * z
			translate 0.3 * x
		}
		rotate 5 * z
		translate <-0.2 - 2*legmove, -1.9 + legmove, 0>
	}
	rotate 10 * sin(clock*2*pi) * z
	scale 1.2
}

#declare body =
union {
	sphere {
		0, 2.6
		translate 0.4 * y
	}
	object {
		leg
		rotate -40 * y
		translate <0.3, -0.5, 1.5>
	}
	object {
		leg
		rotate 40 * y
		translate <0.3, -0.5, -1.5>
	}
}

#declare stomach =
union {
	sphere {
		0, 1
		scale <3.2, 1, 1.6>
	}
	sphere {
		0, 1
		scale <2.5, 1.1, 2.2>
		translate <-0.8, 0.2, 0>
	}
	texture {
		pigment { Yellow }
		normal {
			ripples 1.5
			turbulence 0.1
			scale 0.3
			translate -5 * x
		}
	}
}

#declare spike =
sphere {
	0, 1
	scale <1, 1, 0.2>
	pigment { Firebrick * 1.2 }
}

#declare tail =
union {
	sphere {
		0, 0.5
		scale <1.6, 1, 1>
	}
	cone {
		<0, 0, 0>, 0.5, <8, 0, 0>, 2.4
	}
	#declare i = 0;
	#declare spikes = 6;
	#while (i < spikes)
	object {
		spike
		scale <0.4 + i*0.2/spikes, 0.5 + i*0.3/spikes, 1>
		#declare ix = i*(8-(spikes-i)*0.2)/spikes;
		translate <ix, 0.5 + ix/8*(2.9-0.5), 0>
	}
	#declare i=i+1;
	#end
}

#declare neck =
union {
	cone {
		<0, 0, 0>, 2.4, <6, 0, 0>, 0.8
	}
	#declare i = 0;
	#declare spikes = 5;
	#while (i <= spikes)
	object {
		spike
		scale <0.6 - i*0.2/spikes, 0.8 - i*0.3/spikes, 1>
		#declare ix = i*(5.8+(spikes-i)*0.2)/spikes;
		translate <ix, 2.5 - ix/6*(2.5-0.8), 0>
	}
	#declare i=i+1;
	#end
}


#declare ear =
difference {
	torus {
		0.8, 0.08
		rotate 90 * x
	}
	sphere {
		0, 1.5
		translate <1, -0.5, 0>
	}
	rotate -20 * sin(clock*2*pi) * z
	translate <0.9, 0.5, 0>
}

#declare eye =
union {
	torus {
		// eyebrow
		0.4, 0.15
		rotate 70 * z
	}
	union {
		// eye
		sphere {
			0, 0.4
			scale <0.6, 1, 1>
			pigment { White }
		}
		sphere {
			0, 0.26
			scale <0.6, 1, 1>
			translate <0.11, -0.08, 0>
			pigment { Gray20 }
		}
		finish {
			phong 1
			phong_size 80
			brilliance 5
			reflection 0.2
		}
		rotate 30 * z
		translate 0.18 * x
	}
}

#declare head =
difference {
	union {
		sphere {
			// skull
			0, 1
			scale <1.1, 1, 1>
		}
		object {
			ear
			translate -0.2 * z
			rotate 40 * y
		}
		object {
			ear
			translate 0.2 * z
			rotate -40 * y
		}
		object {
			eye
			translate <0.6, 0.4>
			rotate 35 * y
		}
		object {
			eye
			translate <0.6, 0.4>
			rotate -35 * y
		}
		union {
			sphere {
				// upper jaw
				0, 1
				scale <2, 0.6, 1>
				translate <0.9, -0.3, 0>
			}
			sphere {
				// left nostril
				0, 0.25
				translate <2.4, -0.1, -0.5>
			}
			sphere {
				// right nostril
				0, 0.25
				translate <2.4, -0.1, 0.5>
			}
			rotate -3 * sin(clock*2*pi) * z
		}
		sphere {
			// lower jaw
			0, 1
			scale <2, 0.6, 1>
			rotate -20 * z
			translate <0.9, -0.4, 0>
			rotate +3 * sin(clock*2*pi) * z
		}
	}
	sphere {
		// mouth (hole)
		0, 1
		//scale <2, 0.3, 1>
		scale <2, 0.2 - 0.1 * sin(clock*2*pi), 1>
		rotate -10 * z
		//rotate -(10 + 5 * sin(clock*2*pi)) * z
		translate <2, -0.6, 0>
	}
	union {
		sphere {
			// left nostril
			0, 0.22
			translate <2.6, -0.15, -0.7>
		}
		sphere {
			// right nostril
			0, 0.22
			translate <2.6, -0.15, 0.7>
		}
		rotate -3 * sin(clock*2*pi) * z
		pigment { Red }
	}
}

#declare wing =
union {
	cone {
		<-9, 0, 0>, 1, <0, 0, 0>, 0.6
	}
	sphere {
		0.05, 0.7
	}
	intersection {
		torus {
			7, 0.6
			translate -7 * x
		}
		sphere {
			0, 3.5
		}
		scale <1, 1, 1.5>
		translate <0.83, 0, 5>
		rotate 30 * y
	}
	union {
		sphere {
			0, 0.62
		}
		cone {
			<0, 0, 0>, 0.6, <-2.4, 0, 0>, 0.1
		}
		rotate 50 * y
		translate <5.0, 0, 8.6>
	}
	// membrane
	difference {
		prism {
			linear_sweep
			linear_spline
			-0.1, 0.1,	// y1, y2
			7,		// points
			<0,0>,
			<3,4>,
			<5,9>,
			<3.7,10.4>,
			<-10,5>,
			<-9,0>,
			<0,0>
		}
		sphere {
			0, 1
			scale <13.3, 2, 5.2>
			translate <-10, 0, 10.5>
		}
	}
	rotate 90 * z
	rotate -90 * y
	rotate 10 * cos(clock*2*pi) * z
	translate 9 * y
	scale 0.5
}

// Lizzie, the she-draco-queen (relative of Puff)
union {
	union {
		object {
			tail
			translate <-8.3, 0.1, 0>
			rotate -(5 + 5 * sin(clock*2*pi)) * z
		}
		object {
			body
		}
		object {
			stomach
			rotate (63 + 3 * sin(clock*2*pi)) * z
			translate <2.5, 0.3, 0>
		}
		object {
			neck
			rotate (40 + 3 * sin(clock*2*pi)) * z
			translate <0.6, 0.4, 0>
		}
		// breathing
		#declare breath = 0.1 * sin(clock*2*pi);
		scale <1, 0.8 - breath, 0.9 - breath>
	}
	union {
	object {
		head
		}
		union {
			// teeth
			sphere {
				0, 0.3
				scale <0.3, 1, 0.3>
				translate -0.5 * z
			}
			sphere {
				0, 0.3
				scale <0.3, 1, 0.3>
				translate 0.5 * z
			}
			rotate -15 * z
			translate <2.3, -0.5, 0>
			pigment { White }
		}
		scale 1.2
		translate <5.4, 3.4 * (0.9 - breath), 0>
		rotate 3 * sin(clock*2*pi) * z
	}
	object {
		wing
		translate <1, 1, -0.8>
		rotate  (25 * cos(clock*2*pi) - 25) * y
		rotate -(50 + 15 * sin(clock*2*pi)) * x
		translate 0.8 * y
	}
	object {
		wing
		translate <1, 1, 0.8>
		rotate -(25 * cos(clock*2*pi) - 25) * y
		rotate  (50 + 15 * sin(clock*2*pi)) * x
		translate 0.8 * y
	}
	texture {
		pigment { DarkGreen * 1.2 }
		normal {
			ripples 0.6
			turbulence 0.5
			scale 0.3
		}
		// move ripples center so that wings & tail look good
		translate -2 * x
	}
	finish {
		phong 0.1
		phong_size 10
		//metallic
	}
	translate -1 * y
	/* front: rotate 90 * y */
}
