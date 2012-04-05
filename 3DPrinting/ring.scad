EPS=0.01;

difference() {
    cylinder(h=30, r=40, center = true, $fn=14);

    cylinder(h=35+2*EPS, r=25, center = true, $fn=54);
    translate([20,-5,0]) cube(size=[30,10,20]);
}
