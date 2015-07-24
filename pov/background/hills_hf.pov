

#declare XMIN=-1.3333;
#declare XMAX=1.3333;

#declare YMIN=-1;
#declare YMAX=1;

#declare PREVIEW = 1;

global_settings { hf_gray_16 true }

camera {
    orthographic
    location <(XMAX + XMIN)/2,(YMAX + YMIN)/2,-5>
    look_at  <(XMAX + XMIN)/2,(YMAX + YMIN)/2,0>
    up y*(YMAX - YMIN)
    right x*(XMAX - XMIN)
  }

plane{ <0,0,-1>,0
	texture{ pigment{ spotted
		color_map{
		[0.4 color rgb <0,0,0>]
		[1 color rgb <1,1,1>]
		}
		scale 0.2
		}
		finish{ambient 1}	
		}
}