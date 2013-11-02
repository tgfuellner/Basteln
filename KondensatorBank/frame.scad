/*
    Anordnung der Kondesatoren:

  +---------+   ^
  | O O O  -|   |
  | O O O  +|   |  FrameHeight
  +---------+   v     
  <--------->    
      FrameWidth

    Zwei Zeilen und drei Spalten
*/

$fn=100;
EPS=0.1;

CapDia=33;
TopPlateThickness=40;
GapBetweenCap=1;
BorderThickness=2;
TerminalScrewDia=12+0.5;
TerminalScrewClearing=10;
MontageScrewDia=4;
MontageScrewHeadDia=8;
MontageScrewHeadHeight=3;

CoverThickness=4+0.5;
/////// Configuration end ///////////////

CapRadius=CapDia/2;
TerminalScrewRadius=TerminalScrewDia/2;
MontageScrewRadius=MontageScrewDia/2;
MontageScrewHeadRadius=MontageScrewHeadDia/2;

FrameWidth=3*CapDia + 2*GapBetweenCap + BorderThickness + TerminalScrewDia + 2*TerminalScrewClearing;
FrameHeight=2*CapDia + GapBetweenCap + 2*BorderThickness;

module montageScrew() {
    cylinder(h=TopPlateThickness, r=MontageScrewRadius, center=false);
    hull() {
        cylinder(h=EPS, r=MontageScrewRadius, center=false);
    translate([0,0,-TopPlateThickness-MontageScrewHeadHeight])
        cylinder(h=TopPlateThickness, r=MontageScrewHeadRadius, center=false);
    }
}

module top() {
    difference() {
        cube(size=[FrameWidth,FrameHeight,TopPlateThickness], center=false);

        // Caps
        for (col=[0,1,2], row=[0,1])
            translate([BorderThickness+CapRadius+col*(CapDia+GapBetweenCap)
                       ,BorderThickness+CapRadius+row*(CapDia+GapBetweenCap)
                       ,CoverThickness])
                cylinder(h=TopPlateThickness, r=CapRadius, center=false);

        // Minus Pol
        translate([FrameWidth-TerminalScrewRadius-TerminalScrewClearing
                   ,FrameHeight-TerminalScrewRadius-TerminalScrewClearing
                   ,0])
             cylinder(h=TopPlateThickness, r=TerminalScrewRadius, center=false);

        // Plus Pol
        translate([FrameWidth-TerminalScrewRadius-TerminalScrewClearing
                   ,TerminalScrewRadius+TerminalScrewClearing
                   ,0])
             cylinder(h=TopPlateThickness, r=TerminalScrewRadius, center=false);

        // Montage Screw
        for (col=[0,1])
            translate([BorderThickness+CapDia+GapBetweenCap/2+col*(CapDia+GapBetweenCap)
                   ,FrameHeight/2
                   ,EPS])
               translate([0,0,TopPlateThickness-10]) montageScrew();
        }
}

top();
