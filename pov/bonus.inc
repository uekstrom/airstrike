/*
 * A bonus bubble for the Airstrike game
 *
 * (w) 2003 by Eero Tamminen
 */
difference {
	sphere {
		0, 1
	}
	sphere {
		0, 0.99
	}
	interior { ior 1.4 }
	texture {
		pigment {
			color rgbf <0.99, 0.99, 0.99, 0.99>
		}
		finish {
			ambient 0.1	
			diffuse 0.1
			phong 1
			phong_size 400
			specular 0.8
			roughness 0.003
			irid {
				0.1
				thickness 0.3
				turbulence 0.1
			}
		}
	}
	// scale and center around image
	scale <(XMAX-XMIN)/2, (YMAX-YMIN)/2, 1>
	translate <(XMAX+XMIN)/2, (YMAX+YMIN)/2, 0>
}
