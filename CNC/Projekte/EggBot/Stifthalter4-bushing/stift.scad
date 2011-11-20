// Pen holder
// Uses bushings and a fitting rod from an old CD-Rom drive

EPS = 0.01;
$fs = 0.2;

// I use plywood
BoardThickness = 6;

YDistanceOfPenTipToVerticalEnd = 52;
ServoLiftHeight = 18;

PenDiameter = 11;
PenDipLength = 10;
PenLength = PenDipLength+130;

// Bearing
RodDiameter = 3;
RodLength = 75;
BushingDiameter = 4.5;
BushingLength = 3;

// Slider
SliderBoardThikness = 7;  // Acrylic


// Pen fixing
ThicknessOfJoinWithSlider = 10;
ScrewDiameter = 3;
ScrewNutY = 5.5;
ScrewNutX = 2.5;
MaterialArroundPenHole = 4;
MaterialNeededForScrew = 5;


// Colors
Aluminum = [0.77, 0.77, 0.8];
BlackPaint = [0.2, 0.2, 0.2];
BrightYellow = [1, 1, 0];
Transparent = [0.3, 0.3, 1, 0.8];

module slider() {
    xDistToPenTip = PenDiameter/2 + MaterialArroundPenHole/2;
    xLen = YDistanceOfPenTipToVerticalEnd- xDistToPenTip - BoardThickness - 10;
    zLen = ServoLiftHeight + BoardThickness + 2*BushingLength;
    materialArroundBushing = 2;

    translate([xDistToPenTip,-SliderBoardThikness/2,-zLen/2])
      difference() {
        color(Aluminum) cube(size = [xLen,SliderBoardThikness,zLen]);
        translate([BushingDiameter/2+materialArroundBushing,SliderBoardThikness/2,-RodLength/2+zLen/2])
            cylinder(h=RodLength, r=BushingDiameter/2);
        translate([-EPS,-EPS,BushingLength])
        cube(size=[materialArroundBushing*2 + BushingDiameter, SliderBoardThikness+2*EPS, zLen-2*BushingLength]);
      }

}

module pen() {
    union() {
        translate([0,0,PenDipLength]) color(BlackPaint) cylinder(h = PenLength, r = PenDiameter/2);
        color(BrightYellow) cylinder(h = PenDipLength, r1 = 0.5, r2 = PenDiameter/2);
    }
}

module penFixing() {
    width = PenDiameter+MaterialArroundPenHole*2;

    difference() {
        translate([-width/2, -width/2,-BoardThickness/2])
            union() {
                cube(center = false, size = [width, width, BoardThickness]);
                translate([width,0,0]) 
                    cube(center = false, size = [ThicknessOfJoinWithSlider, width, BoardThickness]);
                translate([-MaterialNeededForScrew,0,0])
                    cube(center = false, size = [MaterialNeededForScrew, width, BoardThickness]);
            }
        // Hole for Pen
        translate([0,0,-BoardThickness/2-EPS]) cylinder(h = BoardThickness+2*EPS, r = PenDiameter/2);

        // Hole for Screw
        translate([-PenDiameter/2-MaterialNeededForScrew-MaterialArroundPenHole-EPS,0,0])
          rotate(a = [0,90,0]) 
            cylinder(h = MaterialArroundPenHole+MaterialNeededForScrew+EPS*40, r = ScrewDiameter/2);

       // Space for ScrewNut
       translate([-PenDiameter/2-MaterialArroundPenHole-ScrewNutX/2, -ScrewNutY/2,-BoardThickness/2-EPS])
            cube([ScrewNutX,ScrewNutY,BoardThickness+2*EPS]);

    }
}

module penSlider() {
    difference() {
      union() {
        linear_extrude(convexity = 10, center = false, height=BoardThickness)
          import(file = "penSlider.dxf", layer = "servoSupport");
        translate([0,0,BoardThickness])
          linear_extrude(convexity = 10, center = false, height=2*BoardThickness)
            import(file = "penSlider.dxf", layer = "outer-cut-full");
      }
      translate([0,0,-EPS])
        linear_extrude(convexity = 10, center = false, height=3*BoardThickness+2*EPS)
          import(file = "penSlider.dxf", layer = "inner-cut-full");

      // Hole for Screw
      translate([-19-PenDiameter/2,0,BoardThickness*1.5]) rotate(a = [0,90,0]) 
          cylinder(h = 20, r = ScrewDiameter/2);
    }
}



pen();
penSlider();
//penFixing();
//slider();
