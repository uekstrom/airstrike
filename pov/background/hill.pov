camera{
	location -5*z
	look_at 0
}

light_source{ <0,7,-5> color rgb <1,1,1>}

#declare WRINKLED_TEXTURE =
    // red and white check
    texture {
      pigment {
        checker
        color rgb<0.920, 0.000, 0.000>
        color rgb<1.000, 1.000, 1.000>
        scale <0.2500, 0.2500, 0.2500>
      }
    }
    // greys to "fade" checks
    texture {
      pigment {
        checker
        color rgbf<0.632, 0.612, 0.688, 0.698>
        color rgbf<0.420, 0.459, 0.520, 0.953>
        turbulence 0.500
        scale <0.2500, 0.2500, 0.2500>
      }
    }
    // the wine stains
    texture {
      pigment {
        spotted
        color_map {
          [ 0.000  color rgb<0.483, 0.165, 0.165> ]
          [ 0.329  color rgbf<1.000, 1.000, 1.000, 1.000> ]
          [ 0.734  color rgbf<1.000, 1.000, 1.000, 1.000> ]
          [ 1.000  color rgb<0.483, 0.165, 0.165> ]
        }
        turbulence 0.500
        frequency 1.500
      }
      normal {
        wrinkles 5.0000
      }
    }
  // wrinkled box
  box {
    <-1, -1, -1>, <1, 1, 1>
    texture {
      WRINKLED_TEXTURE
    } 
  }
