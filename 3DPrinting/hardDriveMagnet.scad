$fa=6;
$fs=0.1;
EPS=0.01;

Height=12;
Width=4;
Depth=23;
MagnetThickness=2.1;
MagnetHeight=10.5;
HandleWidth=2;

difference() {
    union() {
        cube(size=[Width,Depth,Height]);
        translate([Width-EPS,Depth/2-HandleWidth/2,0])
            cube(size=[10,HandleWidth,Height]);
    }
    translate([0.9, -EPS, (Height-MagnetHeight)/2])
        cube(size=[MagnetThickness,Depth-1,MagnetHeight]);
}

/*
difference() {
    union() {
        cylinder(r=45/2, h=BaseHeight);
        translate([0,0,BaseHeight]) cylinder(r=15,h=InnerHeight);
    }
    translate([0,0,-EPS]) cylinder(r=MetalRodRadius, h=BaseHeight+InnerHeight+2*EPS);
    translate([0,0,-EPS]) cylinder(r=BearingR, h=BearingHeight+EPS);
}
*/
