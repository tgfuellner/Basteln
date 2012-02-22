$fa=6;
$fs=0.1;
EPS=0.01;
BaseHeight=14;
InnerHeight=50;
MetalRodRadius=6.5/2;

BearingR=19/2;
BearingHeight=6;

difference() {
    union() {
        cylinder(r=45/2, h=BaseHeight);
        translate([0,0,BaseHeight]) cylinder(r=15,h=InnerHeight);
    }
    translate([0,0,-EPS]) cylinder(r=MetalRodRadius, h=BaseHeight+InnerHeight+2*EPS);
    translate([0,0,-EPS]) cylinder(r=BearingR, h=BearingHeight+EPS);
}
