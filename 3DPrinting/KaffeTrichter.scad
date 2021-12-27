$fa=0.5; // default minimum facet angle is now 0.5
$fs=0.5; // default minimum facet size is now 0.5 mm

eps = 0.1;
// Parametric Funnel by Coasterman

// VARIABLES
bottom_diameter = 75;
bottom_height = 1;
top_diameter = 54;
top_einschub_diameter = 51;
top_height = 4;
width = 3;
cone_height = 26;

// CODE
union()
{
 difference() {
  cylinder(h=bottom_height, r=bottom_diameter/2);
  translate([0,0,-2]) cylinder(h=bottom_height+4, r=bottom_diameter/2 - width);
 }
 translate([0, 0, bottom_height])
 difference() {
  cylinder(h=cone_height, r1=bottom_diameter/2, r2 = top_diameter/2);
  translate([0,0,-eps])
     cylinder(h=cone_height+eps*2, r1=bottom_diameter/2 - width, r2 = top_diameter/2 - width);
 }
 translate([0, 0, cone_height + bottom_height]) {
   difference() {
    cylinder(h=top_height, r=top_diameter/2);
    translate([0,0,-2]) cylinder(h=top_height+4, r=top_diameter/2 - width);
    translate([0,0,+1]) {
        difference() {
          cylinder(h=top_height+4, r=top_diameter/2+eps);
          cylinder(h=top_height+4, r=top_einschub_diameter/2);
        }
    }
  }
 }
}
