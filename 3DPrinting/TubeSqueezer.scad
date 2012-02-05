// PARAMETRIC TUBE SQUEEZER by Coasterman
// Liscensed under Attribution-Creative Commons.
// (c) 2011 by Coasterman.

use <OpenScadFont.scad>;

// VARIABLES
tube_thickness = 1.6;	// Thickness of opening
tube_width = 64;	// Width of opening
squeezer_width = 5;	// Width of walls of squeezer
squeezer_height = 20;	// Height of squeezer
roundness = 0.80; // 1 for rectangular, lower numbers are rounder

tube_inc=2;
tube_inc_inset=3;

EPS=0.1;


intersection() {
  difference() {
    cube([tube_thickness + squeezer_width*2, tube_width + squeezer_width*2/roundness, squeezer_height], true);
    union() {
      translate([0,0,-tube_inc_inset])
    		cube([tube_thickness, tube_width, squeezer_height+EPS], true);
      hull() {
        translate([0,0,squeezer_height/2-tube_inc_inset/2])
    		cube([tube_thickness, tube_width, tube_inc_inset], true);
	    translate([0,0,squeezer_height]) 
      		cube([tube_thickness+tube_inc, tube_width, squeezer_height], center=true);
      }
    } 
  }
  scale(v=[roundness, roundness, roundness]) {
    scale(v=[0.35, 0.65, 0.45]) {
      sphere(tube_width + squeezer_width*2);
    }
  }
}

translate([squeezer_width+tube_thickness-1.5,-27,0]) rotate(90,[0,1,0])
scale([1.5,1.4,1]) {
    fnt_str(["D", "a", "u", "m"],4,2,1);
    translate([0, 8*2.5+1.5,0])
    fnt_str(["o", "o", "s", "e", "r"],5,2,1);
}

