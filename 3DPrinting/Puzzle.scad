// Puzzle

EPS=0.01;
$fs=0.2;


RadiusOfHole=3;
ClearanceRod=0.5;
ClearanceRodLength=2;

Height=20;
Width=10;
Length=80;

MaterialAfterHole=10;

HoleLength=Length-2*MaterialAfterHole;

module thing() {

  translate([0,0,Height/2]) difference() {
    cube(size=[Length,Width,Height], center=true);
    translate([0,Width/2+EPS,-Height/4]) rotate(90,[1,0,0])
        cylinder(h=Width+2*EPS, r=RadiusOfHole);
    translate([0,0,Height/2]) rotate(90, [0,0,1])
        cube(size=[Length,Width,Height], center=true);
    translate([-HoleLength/2,0,Height/4]) rotate(90,[0,1,0])
        cylinder(h=HoleLength, r=RadiusOfHole);
  }
}

module rod() {
    cylinder(h=(HoleLength-Width)/2-ClearanceRodLength, r=RadiusOfHole-ClearanceRod/2);
}

module cut() {
    translate([Width/2-EPS,-Width/2-EPS,-EPS])
        cube(size=[Length,Width+2*EPS,Height+2*EPS], center=false);
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


translate([-Width,-Width,0]) rod();
cutInTwoPartsForAssembly();
translate([Width,-Width,0]) rod();
translate([0,2*Width,0]) cutInTwoPartsForAssembly();
translate([-Width,-Width*2,0]) rod();
translate([Width,-Width*2,0]) rod();
