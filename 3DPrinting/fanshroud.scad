$fn=100;

// inner radius of the tube
iradius = 38/2;
// Wall thickness of the tube
wallthick = 1.2;
// Length of the tube
length = 40.0;
baffle_thick = 0.7;

basethick = 5.0;

gooseneck_off = 10;
gooseneck_r = 2.6;

screw_clearance_r = 3.4/2;
head_clearance_r = 5.41/2;
hole_distance = 31.88/2;
head_clearance_hgt = 5;

wirehole_x = -10;
wirehole_y = 23;

module verticalTube(ir, thickness, height) {
	difference() {
		cylinder(h=height, r=ir+thickness);
		translate([0,0,-1.0])
			cylinder(h=height+2.0, r=ir);
	}
}

module baffled_tube() {
    union() {
	verticalTube(iradius, wallthick, length);
	translate([-(iradius+0.1),0,0])
		cube([2*(iradius+0.1), baffle_thick, length]);
	rotate([0,0,90])
		translate([-(iradius+0.1),0,0])
			cube([2*(iradius+0.1), baffle_thick, length]);
    }
} 

module fourholes(rad, hgt) {
		translate([hole_distance,hole_distance,0])
			cylinder(r=rad, h=hgt);
		translate([hole_distance,-hole_distance,0])
			cylinder(r=rad, h=hgt);
		translate([-hole_distance,hole_distance,0])
			cylinder(r=rad, h=hgt);
		translate([-hole_distance,-hole_distance,0])
			cylinder(r=rad, h=hgt);
}

module base() {
	difference() {
        union() {
		translate([-(iradius+wallthick), -(iradius+wallthick),0])
			union() {
				cube([2*(iradius+wallthick),2*(iradius+wallthick),basethick]);
				translate([iradius+wallthick,2*(iradius+wallthick),0])
					cylinder(r=iradius+wallthick, h=basethick);
			}
		translate([0,iradius+wallthick+(gooseneck_off),basethick])
		    cylinder(r=gooseneck_r+5, h=basethick+5);
        }
		union() {
			translate([0,iradius+wallthick+(gooseneck_off),-1])
				cylinder(r=gooseneck_r, h=basethick+20);
			translate([0,0,-1])
				cylinder(r=iradius, h=basethick+2);
			translate([0,0,-1])
				fourholes(screw_clearance_r, basethick+2);
			translate([wirehole_x, wirehole_y,-1])
				cylinder(r=screw_clearance_r, h=basethick+2);
		}
	}
}

difference() {
	union() {
		baffled_tube();
		base();
	}
	translate([0,0,basethick])
		fourholes(head_clearance_r, head_clearance_hgt);
}
