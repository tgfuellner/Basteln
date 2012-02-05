$fn=100;

thk = 4.4;
ply = 6;
h1 = 18;
top_len = 22;
h2 = h1+thk+top_len;

bracket_len = 26;
brace_len = 3.5;
gooseneck_hole_radius = 2.6;
m3_clearance_hole_r = 3.2/2;

small_hole_inset = 4;
small_hole_backset = 8;

module bracket() {
	linear_extrude(height=bracket_len, convexity = 10, twist = 0)
		polygon(points=[[0,0],[0,h1],[0,h1+thk],[thk+ply,h1+thk],[thk+ply,h2],[thk+ply+thk,h2],[thk+ply+thk,0],[thk+ply,0],[thk+ply,h1],[thk,h1],[thk,0]]);
}

module brace() {
	linear_extrude(height=brace_len, convexity = 10, twist = 0)
		polygon(points=[[0,h1+thk],[thk+ply,h1+thk],[thk+ply,h2]]);
}

module complete_bracket() {
	difference() {
		union() {
			bracket();
			brace();
			translate([6,h2-(top_len/2),bracket_len/2])
				rotate([0,90,0])
						cylinder(r=gooseneck_hole_radius+6, h=thk);
		}
		union() {
			translate([-1,small_hole_backset,small_hole_inset])
				rotate([0,90,0])
						cylinder(r=m3_clearance_hole_r, h=thk+thk+ply+2);
			translate([-1,small_hole_backset,bracket_len-small_hole_inset])
				rotate([0,90,0])
						cylinder(r=m3_clearance_hole_r, h=thk+thk+ply+2);
			translate([-1,h2-(top_len/2),bracket_len/2])
				rotate([0,90,0])
						cylinder(r=gooseneck_hole_radius, h=thk+thk+ply+2);
	
		}
	}
}

translate([-(thk+thk+ply)/2,-h2/2,0])
	complete_bracket();
