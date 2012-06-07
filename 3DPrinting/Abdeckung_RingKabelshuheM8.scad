$fn=32;
EPS=0.01;

MetalZ=24;
hCyl=6;
rCyl=6;


rotate(180,[0,1,0]) difference() {
    hull() {
        translate([0,0,MetalZ/2]) cube(center=true, size=[20,5,MetalZ]);
        translate([0,rCyl-3,-hCyl]) cylinder(r=rCyl,h=hCyl);
    }
    translate([0,0,MetalZ/2-hCyl+EPS]) cube(center=true, size=[14.2,1.1,MetalZ+hCyl+4]);
    translate([0,rCyl-3,-hCyl-EPS]) cylinder(r=4.2,h=hCyl+1);
}
