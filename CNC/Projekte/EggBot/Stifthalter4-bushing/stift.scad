// Pen holder
// Uses bushings and a fitting rod from an old CD-Rom drive

//use </usr/share/openscad/MCAD/servos.scad>

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

// Pen fixing
ThicknessOfJoinWithSlider = 10;
ScrewDiameter = 3;
ScrewNutY = 5.5;
ScrewNutX = 2.5;
MaterialArroundPenHole = 4;
MaterialNeededForScrew = 5;

ServoY = 12.6;

// Colors
Aluminum = [0.77, 0.77, 0.8];
BlackPaint = [0.2, 0.2, 0.2];
BrightYellow = [1, 1, 0];
Transparent = [0.3, 0.3, 1, 0.8];


module pen() {
    union() {
        translate([0,0,PenDipLength]) color(BlackPaint) cylinder(h = PenLength, r = PenDiameter/2);
        color(BrightYellow) cylinder(h = PenDipLength, r1 = 0.5, r2 = PenDiameter/2);
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

      translate([0,0,-EPS])
        linear_extrude(convexity = 10, center = false, height=3.6)
          import(file = "penSlider.dxf", layer = "inner-cut-3.6mm");

      translate([0,0,3*BoardThickness-3.6+EPS])
        linear_extrude(convexity = 10, center = false, height=3.6)
          import(file = "penSlider.dxf", layer = "inner-cut-3.6mm");

      // Hole for Screw
      translate([-19-PenDiameter/2,0,BoardThickness*1.5]) rotate(a = [0,90,0]) 
          cylinder(h = 20, r = ScrewDiameter/2);
    }
}

module servo(angle) {
    translate([0,0,-28.4]) {  
      union() {
        translate([-5.5,-6.3,0]) {
            cube(size=[23,ServoY,22.1]);
            translate([-4.5,0,15.2]) cube(size=[32,12.6,2.3]);
        }
        translate([0,0,22.1]) cylinder(r=5.75, h=4.3);
      }

      rotate(a = [0,0,-angle]) translate([0,-20,26.4])
        linear_extrude(convexity = 10, center = false, height=2)
          import(file = "penSlider.dxf", layer = "servoArm");
    }
}

module horizontalArm() {
    difference() {
        linear_extrude(convexity = 10, center = false, height=BoardThickness)
          import(file = "penSlider.dxf", layer = "horizontal-outer-cut-full");
      translate([0,0,-EPS]) linear_extrude(convexity = 10, center = false, height=BoardThickness+2*EPS)
          import(file = "penSlider.dxf", layer = "horizontal-inner-cut-full");
    }
}

pen();
translate([0,0,BoardThickness]) penSlider();
horizontalArm();

translate ([28,0,-ServoY/2]) rotate(a=[90,180,0]) servo(90);
