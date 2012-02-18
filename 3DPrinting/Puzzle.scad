// Puzzle

EPS=0.01;
$fs=0.2;


RadiusOfHole=3;
ClearanceRod=0.5;
ClearanceRodLength=2;
ClearenceGroove=0.4;

Height=20;
Width=10;
Length=70;

MaterialAfterHole=3;

HoleLength=Length-2*MaterialAfterHole;

JoinSmall=3;
JoinWide=5;
JoinHight=5;

module thing() {

  translate([0,0,Height/2]) difference() {
    cube(size=[Length,Width,Height], center=true);
    translate([0,Width/2+EPS,-Height/4]) rotate(90,[1,0,0])
        cylinder(h=Width+2*EPS, r=RadiusOfHole);
    translate([0,0,Height/2]) rotate(90, [0,0,1])
        cube(size=[Length,Width+ClearenceGroove,Height], center=true);
    translate([-HoleLength/2,0,Height/4]) rotate(90,[0,1,0])
        cylinder(h=HoleLength, r=RadiusOfHole);
  }
}

module rod() {
    R=RadiusOfHole-ClearanceRod/2;

    translate([0,0,4*R/5]) rotate(-90,[0,1,0])
        intersection() {
            cylinder(h=(HoleLength-Width)/2-ClearanceRodLength, r=R);
            translate([R/5,0,0])
                cube(size=[R*2,R*2,HoleLength], center=true);
        }
}

module join() {
    rotate(-90, [0,0,1]) rotate(-90, [1,0,0])
    linear_extrude(height = Length, center = false)
    polygon(points = [ [-JoinSmall/2, 0], [JoinSmall/2,0],
        [JoinWide/2,JoinHight], [-JoinWide/2,JoinHight] ]);
}

module cut() {
    translate([(Width+ClearenceGroove)/2-EPS,-Width/2-EPS,Height/2])
    union() {
        cube(size=[Length,Width+2*EPS,Height+2*EPS], center=false);
        translate([0, (Width+2*EPS)/2,EPS]) join();
    }
}

module cutInTwoPartsForAssembly() {
    translate([0,0,Length/2]) {
        rotate(-90,[0,1,0])
          difference() {
            thing();
            cut();
          }

        translate([10,0,0]) rotate(90,[0,1,0])
          intersection() {
            thing();
            cut();
          }
    }
}


translate([0,-Width,0]) rod();
translate([HoleLength/2,-Width,0]) rod();
translate([0,Width,0]) rod();
translate([HoleLength/2,Width,0]) rod();
cutInTwoPartsForAssembly();
translate([0,2*Width,0]) cutInTwoPartsForAssembly();
