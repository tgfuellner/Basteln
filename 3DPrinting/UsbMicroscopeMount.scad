EPS=0.01;
FN=104;


use <polyScrewThread_r1.scad>

PipeInnerRadius=47/2;
PipeHeight=12;
Wanddicke=5;
GapWidth=2;
JoinBlockWidth=18;


module hexScrewForMount() {
  hex_screw(5.2,  // Outer diameter of the thread
           4,  // Thread step
          55,  // Step shape degrees
          0,  // Length of the threaded section of the screw
         0.3,  // Resolution (face at each 2mm of the perimeter)
           -0,  // Countersink in both ends
          8.2,  // Distance between flats for the hex head
           5.2,  // Height of the hex head (can be zero)
           20,  // Length of the non threaded section of the screw
           0); // Diameter for the non threaded section of the screw
               //   -1 - Same as inner diameter of the thread
               //    0 - Same as outer diameter of the thread
}

module hexScrewForCutout() {
  translate([Wanddicke*1.5+0.5,PipeInnerRadius+Wanddicke+JoinBlockWidth/2,PipeHeight/2]) rotate(a=[0,-90,0])
  hex_screw(4.3,  // Outer diameter of the thread
           4,  // Thread step
          55,  // Step shape degrees
          0,  // Length of the threaded section of the screw
         0.3,  // Resolution (face at each 2mm of the perimeter)
           -0,  // Countersink in both ends
          7.1,  // Distance between flats for the hex head
           5,  // Height of the hex head (can be zero)
           20,  // Length of the non threaded section of the screw
           0); // Diameter for the non threaded section of the screw
               //   -1 - Same as inner diameter of the thread
               //    0 - Same as outer diameter of the thread
}


difference() {
  union() {
    difference() {
        cylinder(h=PipeHeight, r=PipeInnerRadius+Wanddicke, center = false, $fn=200);
      translate([0,0,-EPS])
          cylinder(h=PipeHeight+2*EPS, center = false, $fn=100, r=PipeInnerRadius);
    }
    translate([-Wanddicke,PipeInnerRadius+Wanddicke/2,0])
         cube(size=[Wanddicke*2.5,JoinBlockWidth,PipeHeight], center=false);
    rotate(a=[0,0,180])
      translate([-Wanddicke,PipeInnerRadius+Wanddicke/2,0])
         cube(size=[Wanddicke*2.5,JoinBlockWidth,PipeHeight], center=false);

    translate([Wanddicke+PipeInnerRadius+2,0,PipeHeight/2])
      cube(size=[10,10,PipeHeight], center=true);
  }
  translate([0,0,PipeHeight/2])
      cube(size=[GapWidth,(PipeInnerRadius+Wanddicke+JoinBlockWidth)*2,PipeHeight+EPS*2], center=true);
  hexScrewForCutout();
  rotate(a=[0,0,180])
    hexScrewForCutout();

  translate([PipeInnerRadius-1,0,PipeHeight/2]) rotate([0,90,0])
    hexScrewForMount();
}
