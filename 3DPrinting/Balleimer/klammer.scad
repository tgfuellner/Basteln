EPS=0.01;
HEIGHT=70;
W=10;

difference() {
	union() {
		linear_extrude(height = HEIGHT, center = true, convexity = 10)
		   import (file = "klammer.dxf", layer = "0", $fn=50);

		translate([3,70,-HEIGHT/2])
		rotate(-10,[0,0,1])
		rotate(-90,[1,0,0])
		rotate(-90,[0,0,1])
		linear_extrude(height = 25, center = true, convexity = 10)
		   import (file = "klammer.dxf", layer = "SegBody", $fn=300);
	}

	translate([-W/2,0,0])
	cube([W, 200, HEIGHT+5*EPS], center=true);
}
