$fn=32;
EPS=0.01;

MetalZ=24;
hCyl=6;
rCyl=5;


rotate(180,[0,1,0]) difference() {
    hull() {
        translate([0,0,MetalZ/2]) cube(center=true, size=[20,4,MetalZ]);
        union() {
            translate([-2,rCyl-3,-hCyl]) cylinder(r=rCyl,h=hCyl);
            translate([2,rCyl-3,-hCyl]) cylinder(r=rCyl,h=hCyl);
        }
    }
    translate([0,0,MetalZ/2-hCyl+EPS]) cube(center=true, size=[14.2,1.2,MetalZ+hCyl+4]);
    hull() {
      translate([2,rCyl-3.5,-hCyl-EPS]) cylinder(r=3,h=hCyl+1);
      translate([-2,rCyl-3.5,-hCyl-EPS]) cylinder(r=3,h=hCyl+1);
    }
}
