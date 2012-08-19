$fn=32;
EPS=0.01;
PerimeterWidth=0.7;

BoardWidth=50+2;
BoardLength=100+2;
BoardThickness=1.5;

WallThickness=8*PerimeterWidth;
PlateThickness=7;
ScrewDia=4+1;
ScrewHead=8+2;
ScrewHeadHeight=3;

SpacerWidth=5;

// Screw for fixing Bord on Plate
FixScrewHoleDia=2;
FixScrewHoleDepth=PlateThickness-2;
FixScrewHoleEvery=10;

//////////

TotalWidth=BoardWidth+2*WallThickness;
TotalLength=BoardLength+2*WallThickness;

main();

module main() {
    xDisp=BoardLength/2-ScrewHead;
    yDisp=BoardWidth/2-ScrewHead;

    difference() {
        cube(size=[TotalLength, TotalWidth, PlateThickness], center=true);

        // Holes for Screws
        ScrewHole(0,0);
        ScrewHole(xDisp, yDisp);
        ScrewHole(xDisp, -yDisp);
        ScrewHole(-xDisp, -yDisp);
        ScrewHole(-xDisp, yDisp);

        FixScrewHoles();
    }

    Spacer(1,1);
    Spacer(-1,1);
    Spacer(-1,-1);
    Spacer(1,-1);
    Spacer(0,1);
    Spacer(0,-1);

}

module Spacer(xSide, ySide) {
    xDisp=BoardLength/2-SpacerWidth/2;
    yDisp=BoardWidth/2-SpacerWidth/2;

    translate([xDisp*xSide, yDisp*ySide, SpacerWidth/2+PlateThickness/2-EPS]) {
        cube(size=[SpacerWidth,SpacerWidth,SpacerWidth], center=true);
    }
}

module ScrewHole(xDisplace, yDisplace) {
    translate([xDisplace, yDisplace, 0]) {
        cylinder(r=ScrewDia/2, h=PlateThickness+2*EPS, center=true);

        translate([0,0,ScrewHeadHeight/2+PlateThickness/2-ScrewHeadHeight+EPS])
            cylinder(r=ScrewHead/2, h=ScrewHeadHeight, center=true);
    }
}

module FixScrewHoles() {
    for (x=[0 : FixScrewHoleEvery : BoardLength]) {
        FixScrewHole(x-BoardLength/2, FixScrewHoleDia/2+BoardWidth/2);
        FixScrewHole(x-BoardLength/2, -1/2*(FixScrewHoleDia+BoardWidth));
    }
    for (y=[0 : FixScrewHoleEvery : BoardWidth]) {
        FixScrewHole(FixScrewHoleDia/2+BoardLength/2, y-BoardWidth/2);
        FixScrewHole(-1/2*(FixScrewHoleDia+BoardLength), y-BoardWidth/2);
    }
}

module FixScrewHole(xDisplace, yDisplace) {
    translate([xDisplace, yDisplace, (PlateThickness-FixScrewHoleDepth)/2+EPS]) {
        cylinder(r=FixScrewHoleDia/2, h=FixScrewHoleDepth, center=true);
    }
}
