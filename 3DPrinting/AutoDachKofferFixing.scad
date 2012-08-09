$fn=32;
EPS=0.01;

RScrewHole=4.5/2;

difference(5) {
    cube(size=[34,14,14], center=true);

    translate([-10.5,-4.5-EPS,0])
        cube(size=[7,5,15], center=true);
    translate([10.5,-4.5-EPS,0])
        cube(size=[7,5,15], center=true);

    rotate(90, [1,0,0])
        cylinder(r=RScrewHole, h=15, center=true);

    translate([0,2.5,1.5+EPS])
        cube(size=[7.2,3.5,14/2+4], center=true);
}
