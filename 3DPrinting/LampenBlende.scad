EPS=0.01;
FN=104;

PipeOuterRadius=40;
PipeHeight=80;
Wanddicke=3;
Kabeldicke=5;

difference() {
  union() {
    difference() {
        cylinder(h=PipeHeight, r=PipeOuterRadius, center = false, $fn=14);
        translate([0,0,Wanddicke])
          cylinder(h=PipeHeight, center = false, $fn=FN, r=PipeOuterRadius-Wanddicke);
    }
    cylinder(h=10, r=Kabeldicke, center = false, $fn=14);
  }


  translate([0,0,-EPS])
    cylinder(h=PipeHeight+2*EPS, center = false, $fn=FN, r=Kabeldicke/2.0);
}
