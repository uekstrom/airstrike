/*************************************************************************
            GALAXY INCLUDE FILE FOR PERSISTENCE OF VISION 3.x
**************************************************************************

Created by Chris Colefax, 1 February 1998
Updated 9 August 1998: updated for POV-Ray 3.1

NOTE: This file requires that Galaxy.bg, Galaxy.obj and Galaxy.sf
be properly installed.  See "Galaxy.htm" for more information.

*************************************************************************/

// CHECK VARIABLES AND ASSIGN DEFAULTS
// ***********************************
   #declare _GX_tempver = version; #version 3.0;
   #ifndef (galaxy_seed) #declare galaxy_seed = 0; #end
   #ifndef (galaxy_bg) #declare galaxy_bg = true; #end
   #ifndef (galaxy_objects) #declare galaxy_objects = true; #end
   #ifndef (galaxy_starfield) #declare galaxy_starfield = true; #end
   #ifndef (options_only) #declare options_only = false; #end
   #ifndef (debug_options) #declare debug_options = false; #end

   #ifdef (galaxy_colour) #declare _GX_colour = galaxy_colour * <1, 1, 1>;
      #else #ifdef (galaxy_color) #declare _GX_colour = galaxy_color * <1, 1, 1>;
      #else #declare _GX_colour = <1, 1, 1>; #end #end
   #ifdef (galaxy_colouration) #declare _GX_colouration = galaxy_colouration;
      #else #ifdef (galaxy_coloration) #declare _GX_colouration = galaxy_coloration;
      #else #declare _GX_colouration = .5; #end #end
   #ifdef (galaxy_colour_turb) #declare _GX_colourturb = galaxy_colour_turb * <1, 1, 1>;
      #else #ifdef (galaxy_color_turb) #declare _GX_colourturb = galaxy_color_turb * <1, 1, 1>;
      #else #declare _GX_colourturb = <.2, .2, .2>; #end #end

// SELECT FIRST GALAXY HUE AND CONVERT TO RGB VALUE
// ************************************************
   #declare _GX_rand = seed(galaxy_seed);
   #declare _GX_tempfloat1 = rand(_GX_rand);
   #switch (_GX_tempfloat1)
      #range (0, .333) #declare galaxy_colour1 = <1, _GX_tempfloat1 * 3, 0>; #break
      #range (.333, .444) #declare galaxy_colour1 = <1 - (_GX_tempfloat1 * 9 - 3), 1, 0>; #break
      #range (.444, .555) #declare galaxy_colour1 = <0, 1, _GX_tempfloat1 * 9 - 4>; #break
      #range (.555, .667) #declare galaxy_colour1 = <0, 1 - (_GX_tempfloat1 * 9 - 4), 1>; #break
      #range (.667, .833) #declare galaxy_colour1 = <_GX_tempfloat1 * 6 - 4, 0, 1>; #break
      #range (.833, 1) #declare galaxy_colour1 = <1, 0, 1 - (_GX_tempfloat1 * 6 - 5)>; #end
   #declare galaxy_colour1 = (1 + ((galaxy_colour1 - .5) * _GX_colouration)) * _GX_colour;

// SELECT SECOND GALAXY HUE AND CONVERT TO RGB VALUE
// *************************************************
   #declare _GX_tempfloat1 = _GX_tempfloat1 + ((_GX_colourturb.x + _GX_colourturb.y + _GX_colourturb.z) / (rand(_GX_rand) < .5 ? 3 : -3));
   #declare _GX_tempfloat1 = mod(_GX_tempfloat1, 1) + (_GX_tempfloat1 < 0 ? 1 : 0);
   #switch (_GX_tempfloat1)
      #range (0, .333) #declare galaxy_colour2 = <1, _GX_tempfloat1 * 3, 0>; #break
      #range (.333, .444) #declare galaxy_colour2 = <1 - (_GX_tempfloat1 * 9 - 3), 1, 0>; #break
      #range (.444, .555) #declare galaxy_colour2 = <0, 1, _GX_tempfloat1 * 9 - 4>; #break
      #range (.555, .667) #declare galaxy_colour2 = <0, 1 - (_GX_tempfloat1 * 9 - 4), 1>; #break
      #range (.667, .833) #declare galaxy_colour2 = <_GX_tempfloat1 * 6 - 4, 0, 1>; #break
      #range (.833, 1) #declare galaxy_colour2 = <1, 0, 1 - (_GX_tempfloat1 * 6 - 5)>; #end
   #declare galaxy_colour2 = (.8 + ((galaxy_colour2 - .5) * _GX_colouration)) * _GX_colour;

// SELECT RANDOM OPTIONS
// *********************
   #declare galaxy_pattern_scale = .6 + (rand(_GX_rand) * .8);
   #declare galaxy_pattern_origin = (<rand(_GX_rand), rand(_GX_rand), rand(_GX_rand)> - .5) * 50;
   #declare galaxy_turb_origin = (<rand(_GX_rand), rand(_GX_rand), rand(_GX_rand)> - .5) * 50;
   #declare galaxy_star_colouration = rand(_GX_rand) - .5;
   #declare _GX_colourturb = _GX_colourturb * _GX_colouration;

// SELECT RANDOM GALAXY.BG OPTIONS
// *******************************
   #declare galaxy_bgstars = 1 + int(rand(_GX_rand) * 5.99);
   #declare galaxy_bgnebula = min(1 + int(rand(_GX_rand) * 10), 6);
   #declare galaxy_nebula_sphere = 1 + int(rand(_GX_rand) * 4.99);
   #declare galaxy_nebula_ambient = 1 + (<rand(_GX_rand), rand(_GX_rand), rand(_GX_rand)> - .5) * _GX_colourturb;

// SELECT RANDOM GALAXY.OBJ OPTIONS
// ********************************
   #declare _GX_tempfloat1 = rand(_GX_rand)
   #if (_GX_tempfloat1 <= .6)
      #declare galaxy_cluster_name = concat("Star", str(1 + int(rand(_GX_rand) * 3.99), 0, 0))
      #declare galaxy_cluster_objects = 6 + int(rand(_GX_rand) * 6);
      #declare galaxy_cluster_rotate = min(0, mod(rand(_GX_rand) * 90, 15) - 45);
   #else #declare galaxy_cluster_name = "" #end

   #declare galaxy_object_scale = 1 + rand(_GX_rand);
   #declare galaxy_object_flatten = .1 + rand(_GX_rand) * .5;
   #declare galaxy_object_rotate = (rand(_GX_rand) - .5) * 90;
   #declare galaxy_cluster_scale = galaxy_object_scale * (.3 + (rand(_GX_rand) * .4));

   #switch (_GX_tempfloat1)
   #range (0, .3) #declare galaxy_object_name = concat("Nebula", str(1 + int(rand(_GX_rand) * 5.3), 0, 0)) #break
   #range (.3, .6) #declare galaxy_object_name = concat("Star", str(1 + int(rand(_GX_rand) * 3.99), 0, 0)) #declare galaxy_object_rotate = galaxy_cluster_rotate #break
   #range (.6, .8) #declare galaxy_object_name = concat("Comet", str(1 + int(rand(_GX_rand) * 2.99), 0, 0)) #break
   #else
      #declare galaxy_cluster_name = concat("Meteor", str(1 + int(rand(_GX_rand) * 1.4), 0, 0))
      #declare galaxy_cluster_objects = 4 + int(rand(_GX_rand) * 4);
      #declare galaxy_cluster_rotate = (rand(_GX_rand) - .5) * 90;
      #declare galaxy_object_name = ""
   #end

// SELECT RANDOM STARFIELD OPTIONS
// *******************************
   #declare star_type = int(rand(_GX_rand) * 2.99) + 1;
   #declare star_colour = _GX_colour;
   #declare star_colour_turb = _GX_colourturb;

// WRITE OPTION VALUES TO DEBUG STREAM
// ***********************************
   #if (debug_options != false)
      #debug concat("// GALAXY INCLUDE FILE OPTIONS (galaxy_seed = ", str(galaxy_seed, 0, 0), ")\r\n")
      #debug concat("   #declare galaxy_colour1 = <", str(galaxy_colour1.x, 0, 3), ", ", str(galaxy_colour1.y, 0, 3), ", ", str(galaxy_colour1.z, 0, 3), ">;\r\n")
      #debug concat("   #declare galaxy_colour2 = <", str(galaxy_colour2.x, 0, 3), ", ", str(galaxy_colour2.y, 0, 3), ", ", str(galaxy_colour2.z, 0, 3), ">;\r\n")
      #debug concat("   #declare galaxy_pattern_scale = ", str(galaxy_pattern_scale, 0, 3), ";\r\n")
      #debug concat("   #declare galaxy_pattern_origin = <", str(galaxy_pattern_origin.x, 0, 3), ", ", str(galaxy_pattern_origin.y, 0, 3), ", ", str(galaxy_pattern_origin.z, 0, 3), ">;\r\n")
      #debug concat("   #declare galaxy_turb_origin = <", str(galaxy_turb_origin.x, 0, 3), ", ", str(galaxy_turb_origin.y, 0, 3), ", ", str(galaxy_turb_origin.z, 0, 3), ">;\r\n")
      #debug concat("   #declare galaxy_star_colouration = ", str(galaxy_star_colouration, 0, 3), ";\r\n")
      #debug "\r\n"
      #debug concat("   #declare galaxy_bgstars = ", str(galaxy_bgstars, 0, 0), ";\r\n")
      #debug concat("   #declare galaxy_bgnebula = ", str(galaxy_bgnebula, 0, 0), ";\r\n")
      #debug concat("   #declare galaxy_nebula_sphere = ", str(galaxy_nebula_sphere, 0, 0), ";\r\n")
      #debug concat("   #declare galaxy_nebula_ambient = <", str(galaxy_nebula_ambient.x, 0, 3), ", ", str(galaxy_nebula_ambient.y, 0, 3), ", ", str(galaxy_nebula_ambient.z, 0, 3), ">;\r\n")
      #debug "\r\n"
      #debug concat("   #declare galaxy_object_name = \"", galaxy_object_name, "\"\r\n")
      #if (strlen (galaxy_object_name) > 0)
         #debug concat("   #declare galaxy_object_scale = ", str(galaxy_object_scale, 0, 3), ";\r\n")
         #debug concat("   #declare galaxy_object_flatten = ", str(galaxy_object_flatten, 0, 3), ";\r\n")
         #debug concat("   #declare galaxy_object_rotate = ", str(galaxy_object_rotate, 0, 1), ";\r\n")
      #end
      #debug concat("   #declare galaxy_cluster_name = \"", galaxy_cluster_name, "\"\r\n")
      #if (strlen (galaxy_cluster_name) > 0)
         #debug concat("   #declare galaxy_cluster_objects = ", str(galaxy_cluster_objects, 0, 0), ";\r\n")
         #debug concat("   #declare galaxy_cluster_scale = ", str(galaxy_cluster_scale, 0, 3), ";\r\n")
         #debug concat("   #declare galaxy_cluster_rotate = ", str(galaxy_cluster_rotate, 0, 1), ";\r\n")
      #end
      #debug "\r\n"
      #debug concat("   #declare star_type = ", str(star_type, 0, 0), ";\r\n")
      #debug concat("   #declare star_colour = <", str(star_colour.x, 0, 3), ", ", str(star_colour.y, 0, 3), ", ", str(star_colour.z, 0, 3), ">;\r\n")
      #debug concat("   #declare star_colour_turb = <", str(star_colour_turb.x, 0, 3), ", ", str(star_colour_turb.y, 0, 3), ", ", str(star_colour_turb.z, 0, 3), ">;\r\n")
      #debug "\r\n\r\n"
   #end

// CREATE GALAXY BACKGROUND, OBJECTS AND STARFIELD
// ***********************************************
   #if (options_only = false)
      #if (galaxy_bg != false) #include "galaxy/GALAXY.BG" #end
      #if (galaxy_objects != false) #include "galaxy/GALAXY.OBJ" #end
      #if (galaxy_starfield != false) #include "galaxy/GALAXY.SF" #end
   #end

   #declare galaxy_objects = false;
   #declare galaxy_starfield = false;

// SELECT NEW GALAXY SEED (FOR SUBSQUENT GALAXY.INC INCLUSIONS)
// ************************************************************
   #declare galaxy_seed = (rand(_GX_rand) - .5) * 2e6;
   #version _GX_tempver;
