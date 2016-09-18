HeightZwischenStueck=0.6;
HeightWurzel=50;
HeightFitter=11;


translate([1.5,0,HeightZwischenStueck]) rotate_extrude($fn=20) import("wurzel.dxf");
translate([0,0,0]) cylinder(r=7,h=HeightZwischenStueck);

translate([17,0,0]) {
	cylinder(r=7,h=HeightZwischenStueck);
	translate([0,0,HeightFitter/2 + HeightZwischenStueck]) {
	  difference() {
	     union() {
	       cube([9.8,1.2,5], center=true);
	       cube([9.4,1.2,HeightFitter], center=true);
	     }
	     cube([6.5,1.5,8], center=true);
	  }

	  rotate([0,0,90]) {
	     cube([9.3,1.2,HeightFitter], center=true);
	     translate([0,0,2]) cube([10,1.2,2], center=true);
	     translate([0,0,-2]) cube([10,1.2,2], center=true);
	  }
	}
}
