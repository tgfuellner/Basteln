EPS=0.01;
HEIGHT=60;     // Check with dxf
W=10;

difference() {
        difference() {
	   union() {
		linear_extrude(height = HEIGHT, center = true, convexity = 10)
		   import (file = "klammer.dxf", layer = "0", $fn=50);

		translate([11,70,-HEIGHT/2])
		rotate(-10,[0,0,1])
		rotate(-90,[1,0,0])
		rotate(-90,[0,0,1])
		  linear_extrude(height = 25, center = true, convexity = 10)
		   import (file = "klammer.dxf", layer = "SegBody", $fn=300);

          }
		translate([12,70,-HEIGHT/2])
		rotate(-10,[0,0,1])
		rotate(-90,[1,0,0])
		rotate(-90,[0,0,1])
          translate([0,0,9+EPS])
		  linear_extrude(height = 7, center = true, convexity = 10)
		   import (file = "klammer.dxf", layer = "SegCut", $fn=300);
	}

	translate([-19,97,0])
	rotate(29,[0,0,1])
	cube([W, 100, HEIGHT+5*EPS], center=true);
}
